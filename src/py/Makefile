# Simple Makefile to build libchess for Windows (MinGW) or POSIX
CC ?= gcc
CFLAGS ?= -O2 -std=c11 -Wall -Wextra
LDFLAGS ?=
BUILD_DIR ?= build

# Prefer local bitboard.c in this directory if present, otherwise fall back to ../c
ifneq (,$(wildcard bitboard.c))
SRC = chessapi.c bitboard.c
else
SRC = chessapi.c ../c/bitboard.c
endif

all: check-mingw

# Default: try to build a Windows DLL using MinGW (when available)
check-mingw: $(BUILD_DIR)/libchess.dll

$(BUILD_DIR)/libchess.dll: $(SRC) | $(BUILD_DIR)
	$(CC) -shared $(CFLAGS) -DBUILDING_CHESSAPI -o $@ $(SRC) -Wl,--out-implib,$(BUILD_DIR)/libchess.a $(LDFLAGS)

# POSIX shared object
$(BUILD_DIR)/libchess.so: $(SRC) | $(BUILD_DIR)
	$(CC) -fPIC -shared $(CFLAGS) -DBUILDING_CHESSAPI -o $@ $(SRC) $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(BUILD_DIR)/libchess.* $(BUILD_DIR)/*.o

.PHONY: all clean check-mingw
