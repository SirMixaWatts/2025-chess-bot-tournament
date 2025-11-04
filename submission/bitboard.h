#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifndef DLLEXPORT
    #define DLLEXPORT
#endif

typedef uint64_t BitBoard;

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT void dump_bitboard(BitBoard board, char *buffer);
DLLEXPORT BitBoard bb_slide_n(BitBoard board);
DLLEXPORT BitBoard bb_slide_ne(BitBoard board);
DLLEXPORT BitBoard bb_slide_e(BitBoard board);
DLLEXPORT BitBoard bb_slide_se(BitBoard board);
DLLEXPORT BitBoard bb_slide_s(BitBoard board);
DLLEXPORT BitBoard bb_slide_sw(BitBoard board);
DLLEXPORT BitBoard bb_slide_w(BitBoard board);
DLLEXPORT BitBoard bb_slide_nw(BitBoard board);
DLLEXPORT BitBoard bb_flood_n(BitBoard board, BitBoard empty, bool captures);
DLLEXPORT BitBoard bb_flood_ne(BitBoard board, BitBoard empty, bool captures);
DLLEXPORT BitBoard bb_flood_e(BitBoard board, BitBoard empty, bool captures);
DLLEXPORT BitBoard bb_flood_se(BitBoard board, BitBoard empty, bool captures);
DLLEXPORT BitBoard bb_flood_s(BitBoard board, BitBoard empty, bool captures);
DLLEXPORT BitBoard bb_flood_sw(BitBoard board, BitBoard empty, bool captures);
DLLEXPORT BitBoard bb_flood_w(BitBoard board, BitBoard empty, bool captures);
DLLEXPORT BitBoard bb_flood_nw(BitBoard board, BitBoard empty, bool captures);
DLLEXPORT BitBoard bb_blocker_n(BitBoard board, BitBoard empty);
DLLEXPORT BitBoard bb_blocker_ne(BitBoard board, BitBoard empty);
DLLEXPORT BitBoard bb_blocker_e(BitBoard board, BitBoard empty);
DLLEXPORT BitBoard bb_blocker_se(BitBoard board, BitBoard empty);
DLLEXPORT BitBoard bb_blocker_s(BitBoard board, BitBoard empty);
DLLEXPORT BitBoard bb_blocker_sw(BitBoard board, BitBoard empty);
DLLEXPORT BitBoard bb_blocker_w(BitBoard board, BitBoard empty);
DLLEXPORT BitBoard bb_blocker_nw(BitBoard board, BitBoard empty);

#ifdef __cplusplus
}
#endif