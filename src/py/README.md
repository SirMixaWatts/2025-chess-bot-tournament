# Getting started (Python language version)

## Setup
- Ensure you have a working Python version installed
  - The API was tested with Python 3.12.11, so this is the recommended version.
- Copy `pychessapi.py`, `example_bot.py` into a directory
- Copy the bitboard and chessapi `.c` files from the C section of this Git repo into the same directory
- Build the Chess API into a library: `libchess.so` (`libchess.dll` on Windows)
  - The command to do so might look something like this: `cc -o libchess.so -shared bitboard.c chessapi.c -fPIC`
  - It *must* be named `libchess.so`/`libchess.dll`

Windows build notes
-------------------

You can build a Windows `libchess.dll` using MSYS2/MinGW-w64 (recommended) or MSVC.

MinGW/MSYS2 (gcc)
```
# Use the mingw64 shell
x86_64-w64-mingw32-gcc -shared -O2 -std=c11 -DBUILDING_CHESSAPI -o libchess.dll chessapi.c ..\c\bitboard.c -Wl,--out-implib,libchess.a
```

MSVC (cl)
```
cl /LD /O2 /DBUILDING_CHESSAPI chessapi.c ..\c\bitboard.c /Fe:libchess.dll
```

Notes:
- The source uses C11-like thread/mutex APIs; the repo includes a small `platform_threads.h` shim to provide equivalents on Windows.
- Be sure to compile/link `bitboard.c` (or the C copy of `bitboard.c`) together with `chessapi.c`.
- When building for Python you may need an import library (`.lib`) or to load symbols dynamically; using the provided `CHESSAPI_API` export macro and `-DBUILDING_CHESSAPI` ensures symbols are exported.
- Modify `example_bot.py` with your own code
- Run your code!
  - Ensure `libchess.so`/`libchess.dll` is in the same directory as your bot when run
  - Tip: Easiest to test by adding as a bot to a compatible chess program

Be sure to refer to the [docs](https://github.com/shiro-nya/2025-chess-bot-tournament/wiki/Chess-API-(Python)) to see how to use the API!
