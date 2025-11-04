#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "bitboard.h"

#if defined(_WIN32) || defined(__CYGWIN__)
    #if defined(BUILDING_CHESSAPI)
        #define CHESSAPI_API __declspec(dllexport)
    #else
        #define CHESSAPI_API __declspec(dllimport)
    #endif
#else
    #define CHESSAPI_API
#endif

#ifndef DLLEXPORT
#define DLLEXPORT CHESSAPI_API
#endif

typedef enum { WHITE, BLACK } PlayerColor;
typedef enum { PAWN = 1, BISHOP, KNIGHT, ROOK, QUEEN, KING } PieceType;
typedef enum { GAME_CHECKMATE = -1, GAME_NORMAL, GAME_STALEMATE } GameState;
typedef struct Board Board;

typedef struct {
    BitBoard from;
    BitBoard to;
    uint8_t promotion;
    bool capture;
    bool castle;
} Move;

#ifdef __cplusplus
extern "C" {
#endif

CHESSAPI_API Board *chess_get_board();
CHESSAPI_API Board *chess_clone_board(Board *board);
CHESSAPI_API Move *chess_get_legal_moves(Board *board, int *len);
DLLEXPORT int chess_get_legal_moves_inplace(Board *board, Move *moves, size_t maxlen_moves);
CHESSAPI_API bool chess_is_white_turn(Board *board);
CHESSAPI_API bool chess_is_black_turn(Board *board);
CHESSAPI_API void chess_skip_turn(Board *board);
CHESSAPI_API bool chess_in_check(Board *board);
CHESSAPI_API bool chess_in_checkmate(Board *board);
CHESSAPI_API bool chess_in_draw(Board *board);
CHESSAPI_API bool chess_can_kingside_castle(Board *board, PlayerColor color);
CHESSAPI_API bool chess_can_queenside_castle(Board *board, PlayerColor color);
CHESSAPI_API GameState chess_get_game_state(Board *board);
CHESSAPI_API uint64_t chess_zobrist_key(Board *board);
CHESSAPI_API void chess_make_move(Board *board, Move move);
CHESSAPI_API void chess_undo_move(Board *board);
CHESSAPI_API void chess_free_board(Board *board);
CHESSAPI_API BitBoard chess_get_bitboard(Board *board, PlayerColor color, PieceType piece_type);
CHESSAPI_API int chess_get_full_moves(Board *board);
CHESSAPI_API int chess_get_half_moves(Board *board);
CHESSAPI_API void chess_push(Move move);
CHESSAPI_API void chess_done();
DLLEXPORT Board* chess_board_from_fen(const char *fen);
DLLEXPORT void chess_dump_move(char *buffer, Move move);
CHESSAPI_API uint64_t chess_get_time_millis();
CHESSAPI_API uint64_t chess_get_opponent_time_millis();
CHESSAPI_API uint64_t chess_get_elapsed_time_millis();
CHESSAPI_API PieceType chess_get_piece_from_index(Board *board, int index);
CHESSAPI_API PieceType chess_get_piece_from_bitboard(Board *board, BitBoard bitboard);
CHESSAPI_API PlayerColor chess_get_color_from_index(Board *board, int index);
CHESSAPI_API PlayerColor chess_get_color_from_bitboard(Board *board, BitBoard bitboard);
CHESSAPI_API int chess_get_index_from_bitboard(BitBoard bitboard);
CHESSAPI_API BitBoard chess_get_bitboard_from_index(int index);
CHESSAPI_API void chess_init(void);
CHESSAPI_API void chess_shutdown(void);
CHESSAPI_API Move chess_get_opponent_move();
CHESSAPI_API void chess_free_moves_array(Move *moves);

#ifdef __cplusplus
}
#endif