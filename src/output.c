#include "output.h"
#include <stdlib.h>
#include <unistd.h>

// escape sequences start with \x1b (1 byte) followed by a [ (1 byte)
// escape sequence commands take arguments before the command

void clear_screen() {
    // clear screen
    write(STDOUT_FILENO, "\x1b[2J", 4);
    // reposition cursor to top left
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void draw_rows(int rows) {
    for (int i = 0; i < rows - 1; ++i) {
        write(STDOUT_FILENO, "~\r\n", 3);
    }
    write(STDOUT_FILENO, "~", 1);
}

void refresh_screen(CFG* cfg) {
    clear_screen();
    draw_rows(cfg->screen_rows);
    write(STDOUT_FILENO, "\x1b[H", 3);
}