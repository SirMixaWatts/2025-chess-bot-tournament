import ctypes
import os

lib_path = os.path.dirname(__file__)
dll_names = ['libchess.dll', 'build/libchess.dll', 'libchess.so', 'build/libchess.so']

for name in dll_names:
    path = os.path.join(lib_path, name)
    if os.path.exists(path):
        print('Trying', path)
        lib = ctypes.CDLL(path)
        # initialize API
        if hasattr(lib, 'chess_init'):
            lib.chess_init()
        # get board and call a couple of functions
        # Try to call chess_get_full_moves (int function)
        try:
            lib.chess_get_full_moves.restype = ctypes.c_int
            lib.chess_get_full_moves.argtypes = [ctypes.c_void_p]
            lib.chess_get_board.restype = ctypes.c_void_p
            lib.chess_get_board.argtypes = []
            board = lib.chess_get_board()
            print('chess_get_board ->', board)
            if board:
                print('chess_get_full_moves(board) ->', lib.chess_get_full_moves(board))
                try:
                    lib.chess_free_board.argtypes = [ctypes.c_void_p]
                    lib.chess_free_board(board)
                except Exception:
                    pass
            else:
                print('no board returned')
        except Exception as e:
            print('Call failed:', e)
        # shutdown if available
        if hasattr(lib, 'chess_shutdown'):
            try:
                lib.chess_shutdown()
            except Exception:
                pass
        break
else:
    print('No built library found in', lib_path)
