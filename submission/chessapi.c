#include "chessapi.h"
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
    #include <windows.h>
    #include <process.h>

    // Inline the threading compatibility shim
    typedef HANDLE thrd_t;
    typedef CRITICAL_SECTION mtx_t;
    typedef CONDITION_VARIABLE cnd_t;

    #define mtx_plain 0
    #define mtx_timed 1
    #define mtx_recursive 2

    static inline int thrd_create(thrd_t *thr, int (*func)(void*), void *arg) {
        uintptr_t handle = _beginthreadex(NULL, 0, (unsigned (__stdcall *)(void *))func, arg, 0, NULL);
        if (handle == 0) return -1;
        *thr = (HANDLE)handle;
        return 0;
    }

    static inline int mtx_init(mtx_t *mtx, int type) {
        (void) type;
        InitializeCriticalSection(mtx);
        return 0;
    }

    static inline void mtx_destroy(mtx_t *mtx) {
        DeleteCriticalSection(mtx);
    }

    static inline int mtx_lock(mtx_t *mtx) {
        EnterCriticalSection(mtx);
        return 0;
    }

    static inline int mtx_unlock(mtx_t *mtx) {
        LeaveCriticalSection(mtx);
        return 0;
    }

    static inline int cnd_init(cnd_t *cv) {
        InitializeConditionVariable(cv);
        return 0;
    }

    static inline int cnd_signal(cnd_t *cv) {
        WakeConditionVariable(cv);
        return 0;
    }

    static inline int cnd_wait(cnd_t *cv, mtx_t *mtx) {
        return SleepConditionVariableCS(cv, mtx, INFINITE) ? 0 : -1;
    }

#else
    #if __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
        #include <threads.h>
    #else
        #include <pthread.h>
    #endif
#endif

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define CHESS_BOT_NAME getenv("CHESS_BOT_NAME") ? getenv("CHESS_BOT_NAME") : "Smart Bot"
#define BOT_AUTHOR_NAME getenv("BOT_AUTHOR_NAME") ? getenv("BOT_AUTHOR_NAME") : "AI Team"

typedef struct {
    volatile int locks;
    mtx_t locks_mutex;
    cnd_t wait_cnd;
} Semaphore;

void semaphore_init(Semaphore *sem, int locks) {
    sem->locks = locks;
    mtx_init(&sem->locks_mutex, mtx_plain);
    cnd_init(&sem->wait_cnd);
}

void semaphore_post(Semaphore *sem) {
    mtx_lock(&sem->locks_mutex);
    sem->locks++;
    mtx_unlock(&sem->locks_mutex);
    cnd_signal(&sem->wait_cnd);
}

void semaphore_wait(Semaphore *sem) {
    mtx_lock(&sem->locks_mutex);
    while (sem->locks == 0) {
        cnd_wait(&sem->wait_cnd, &sem->locks_mutex);
    }
    sem->locks--;
    mtx_unlock(&sem->locks_mutex);
}

typedef struct {
    thrd_t uci_thread;
    Board *shared_board;
    uint64_t wtime;
    uint64_t btime;
    clock_t turn_started_time;
    Move latest_pushed_move;
    Move latest_opponent_move;
    mtx_t mutex;
    Semaphore intermission_mutex;
} InternalAPI;

static InternalAPI *API = NULL;

// Material evaluation weights
#define W_PAWN 1
#define W_KNIGHT 3
#define W_BISHOP 3
#define W_ROOK 5
#define W_QUEEN 9

static int evaluate_material(Board *board) {
    int w = 0, b = 0;
    w += __builtin_popcountll(board->bb_white_pawn) * W_PAWN
       + __builtin_popcountll(board->bb_white_knight) * W_KNIGHT
       + __builtin_popcountll(board->bb_white_bishop) * W_BISHOP
       + __builtin_popcountll(board->bb_white_rook) * W_ROOK
       + __builtin_popcountll(board->bb_white_queen) * W_QUEEN;
    b += __builtin_popcountll(board->bb_black_pawn) * W_PAWN
       + __builtin_popcountll(board->bb_black_knight) * W_KNIGHT
       + __builtin_popcountll(board->bb_black_bishop) * W_BISHOP
       + __builtin_popcountll(board->bb_black_rook) * W_ROOK
       + __builtin_popcountll(board->bb_black_queen) * W_QUEEN;
    return w - b;
}

// Forward declarations
static void start_chess_api(void);
static Board *interface_get_board();
static Move *get_legal_moves(Board *board, int *len);
static BitBoard *get_pseudo_legal_moves(Board *board, bool white, bool all_attacked, BitBoard exclude, bool exclude_pawn_moves);
static void make_move(Board *board, Move move);
static void undo_move(Board *board);
static void interface_push(Move move);
static void interface_done();
static void uci_finished_searching();
static bool in_check(Board *board, bool white);

static Move choose_move(Board *snapshot) {
    Move chosen; memset(&chosen, 0, sizeof(chosen));
    if (snapshot == NULL) return chosen;

    int num_moves = 0;
    Move *moves = get_legal_moves(snapshot, &num_moves);
    if (num_moves <= 0 || moves == NULL) {
        if (moves) free(moves);
        return chosen;
    }

    // Get opponent's attack map
    BitBoard *opp_moves = get_pseudo_legal_moves(snapshot, !snapshot->whiteToMove, true, 0, true);
    BitBoard opp_attacks = 0;
    for (int d=0; d<16; d++) opp_attacks |= opp_moves[d];
    free(opp_moves);

    // First look for high-value captures
    int best_idx = -1, best_value = -10000;
    for (int i=0; i<num_moves; i++) {
        Move m = moves[i];
        if (m.capture) {
            make_move(snapshot, m);
            PieceType pt = chess_get_piece_from_bitboard(snapshot, m.to);
            int val = (pt==QUEEN?9:pt==ROOK?5:pt==BISHOP||pt==KNIGHT?3:pt==PAWN?1:0);
            undo_move(snapshot);
            if (val > best_value) { best_value = val; best_idx = i; }
        }
    }
    if (best_idx >= 0) {
        chosen = moves[best_idx];
    } else {
        // No good captures, try to move attacked pieces
        BitBoard our_pieces = snapshot->whiteToMove
            ? (snapshot->bb_white_rook | snapshot->bb_white_knight | snapshot->bb_white_bishop | 
               snapshot->bb_white_queen | snapshot->bb_white_king | snapshot->bb_white_pawn)
            : (snapshot->bb_black_rook | snapshot->bb_black_knight | snapshot->bb_black_bishop | 
               snapshot->bb_black_queen | snapshot->bb_black_king | snapshot->bb_black_pawn);

        for (int i=0; i<num_moves; i++) {
            Move m = moves[i];
            if ((m.from & our_pieces & opp_attacks) != 0) {
                chosen = m;
                break;
            }
        }
        
        // If still no move chosen, try checks
        if (chosen.from == 0) {
            for (int i=0; i<num_moves; i++) {
                Move m = moves[i];
                make_move(snapshot, m);
                if (in_check(snapshot, !snapshot->whiteToMove)) {
                    chosen = m;
                    undo_move(snapshot);
                    break;
                }
                undo_move(snapshot);
            }
        }

        // Last resort: random move
        if (chosen.from == 0) chosen = moves[rand() % num_moves];
    }

    free(moves);
    return chosen;
}

// Rest of your original chessapi.c code follows, with UCI stripped down to core commands
// and personality-related code removed...
