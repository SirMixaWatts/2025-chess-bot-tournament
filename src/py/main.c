#include "chessapi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    // Start the chess API which launches the UCI listener in a separate thread
    chess_init();

    // The UCI listener reads from stdin using scanf in its own thread. The main
    // thread should simply wait until stdin is closed or the GUI sends "quit",
    // which causes the UCI thread to call exit(0). We'll block on reading from
    // stdin to keep the process alive.
    int c;
    while ((c = getchar()) != EOF) {
        ; // consume input in main thread; uci thread is handling commands
    }

    // If stdin closed, perform shutdown and exit gracefully
    chess_shutdown();
    return 0;
}
