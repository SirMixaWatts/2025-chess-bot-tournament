#include "chessapi.h"
#include <stdio.h>
#include <stdlib.h>
// Cross-platform sleep
#if defined(_WIN32)
# include <windows.h>
# define SLEEP_MS(ms) Sleep(ms)
#else
# include <unistd.h>
# define SLEEP_MS(ms) usleep((ms) * 1000)
#endif

int main(int argc, char **argv) {
    // Start the chess API which launches the UCI listener in a separate thread
    chess_init();

    // Do not consume stdin here; the UCI listener runs in its own thread and
    // must receive the GUI's commands. Keep the main thread alive with a
    // simple sleep loop. Quit will usually be handled by the UCI thread which
    // calls exit(0), but if stdin closes we'll shutdown gracefully.
    while (1) {
        SLEEP_MS(1000);
        // If you'd like a graceful shutdown hook later, add signal handling
        // and call chess_shutdown() here.
    }

    // unreachable in current design, kept for completeness
    chess_shutdown();
    return 0;
}
