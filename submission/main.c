#include "chessapi.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // Initialize chess API and enter UCI mode
    chess_init();
    
    // UCI protocol requires the process to stay alive and handle stdin
    char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        // Process will exit via UCI 'quit' command
    }
    
    chess_shutdown();
    return 0;
}