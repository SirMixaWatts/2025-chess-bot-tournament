#include "bitboard.h"

static inline BitBoard bb_slide_n(BitBoard board) { return board << 8; }
static inline BitBoard bb_slide_ne(BitBoard board) { return (board & ~0x8080808080808080ull) << 9; }
static inline BitBoard bb_slide_e(BitBoard board) { return (board & ~0x8080808080808080ull) << 1; }
static inline BitBoard bb_slide_se(BitBoard board) { return (board & ~0x8080808080808080ull) >> 7; }
static inline BitBoard bb_slide_s(BitBoard board) { return board >> 8; }
static inline BitBoard bb_slide_sw(BitBoard board) { return (board & ~0x0101010101010101ull) >> 9; }
static inline BitBoard bb_slide_w(BitBoard board) { return (board & ~0x0101010101010101ull) >> 1; }
static inline BitBoard bb_slide_nw(BitBoard board) { return (board & ~0x0101010101010101ull) << 7; }

static inline BitBoard bb_flood_n(BitBoard board, BitBoard empty, bool captures) {
    BitBoard flood = 0;
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_n(curr);
        if (captures) flood |= next;
        curr = next & empty;
        flood |= curr;
    }
    return flood;
}

static inline BitBoard bb_flood_ne(BitBoard board, BitBoard empty, bool captures) {
    BitBoard flood = 0;
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_ne(curr);
        if (captures) flood |= next;
        curr = next & empty;
        flood |= curr;
    }
    return flood;
}

static inline BitBoard bb_flood_e(BitBoard board, BitBoard empty, bool captures) {
    BitBoard flood = 0;
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_e(curr);
        if (captures) flood |= next;
        curr = next & empty;
        flood |= curr;
    }
    return flood;
}

static inline BitBoard bb_flood_se(BitBoard board, BitBoard empty, bool captures) {
    BitBoard flood = 0;
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_se(curr);
        if (captures) flood |= next;
        curr = next & empty;
        flood |= curr;
    }
    return flood;
}

static inline BitBoard bb_flood_s(BitBoard board, BitBoard empty, bool captures) {
    BitBoard flood = 0;
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_s(curr);
        if (captures) flood |= next;
        curr = next & empty;
        flood |= curr;
    }
    return flood;
}

static inline BitBoard bb_flood_sw(BitBoard board, BitBoard empty, bool captures) {
    BitBoard flood = 0;
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_sw(curr);
        if (captures) flood |= next;
        curr = next & empty;
        flood |= curr;
    }
    return flood;
}

static inline BitBoard bb_flood_w(BitBoard board, BitBoard empty, bool captures) {
    BitBoard flood = 0;
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_w(curr);
        if (captures) flood |= next;
        curr = next & empty;
        flood |= curr;
    }
    return flood;
}

static inline BitBoard bb_flood_nw(BitBoard board, BitBoard empty, bool captures) {
    BitBoard flood = 0;
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_nw(curr);
        if (captures) flood |= next;
        curr = next & empty;
        flood |= curr;
    }
    return flood;
}

// Get first blocking piece in direction
static inline BitBoard bb_blocker_n(BitBoard board, BitBoard empty) {
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_n(curr);
        if (!(next & empty)) return next;
        curr = next & empty;
    }
    return 0;
}

static inline BitBoard bb_blocker_ne(BitBoard board, BitBoard empty) {
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_ne(curr);
        if (!(next & empty)) return next;
        curr = next & empty;
    }
    return 0;
}

static inline BitBoard bb_blocker_e(BitBoard board, BitBoard empty) {
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_e(curr);
        if (!(next & empty)) return next;
        curr = next & empty;
    }
    return 0;
}

static inline BitBoard bb_blocker_se(BitBoard board, BitBoard empty) {
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_se(curr);
        if (!(next & empty)) return next;
        curr = next & empty;
    }
    return 0;
}

static inline BitBoard bb_blocker_s(BitBoard board, BitBoard empty) {
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_s(curr);
        if (!(next & empty)) return next;
        curr = next & empty;
    }
    return 0;
}

static inline BitBoard bb_blocker_sw(BitBoard board, BitBoard empty) {
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_sw(curr);
        if (!(next & empty)) return next;
        curr = next & empty;
    }
    return 0;
}

static inline BitBoard bb_blocker_w(BitBoard board, BitBoard empty) {
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_w(curr);
        if (!(next & empty)) return next;
        curr = next & empty;
    }
    return 0;
}

static inline BitBoard bb_blocker_nw(BitBoard board, BitBoard empty) {
    BitBoard curr = board;
    while (curr) {
        BitBoard next = bb_slide_nw(curr);
        if (!(next & empty)) return next;
        curr = next & empty;
    }
    return 0;
}