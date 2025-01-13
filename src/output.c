#include "output.h"
#include "textbuffer.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// escape sequences start with \x1b (1 byte) followed by a [ (1 byte)
// escape sequence commands take arguments before the command

void clear_screen() {
    // clear screen
    write(STDOUT_FILENO, "\x1b[2J", 4);
    // reposition cursor to top left
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void draw_rows(TEXTBUFFER* tb, int rows) {
    write(STDOUT_FILENO, "\x1b[H", 3);
    tb_append(tb, "\x1b[K", 3);
    for (int i = 0; i < rows - 1; ++i) {
        tb_append(tb, "~\r\n", 3);
        // erase everything right of cursor on current line
        tb_append(tb, "\x1b[K", 3);
    }
    tb_append(tb, "~", 1);
}

void refresh_screen(CFG* cfg) {
    TEXTBUFFER tb = {NULL, 0};
    // hide cursor
    tb_append(&tb, "\x1b[?25l", 6);

    draw_rows(&tb, cfg->screen_rows);
    
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "\x1b[%d;%dH", cfg->cy + 1, cfg->cx + 1);
    tb_append(&tb, buffer, strlen(buffer));

    // show cursor
    tb_append(&tb, "\x1b[?25h", 6);
    write(STDOUT_FILENO, tb.buffer, tb.len);
    tb_free(&tb);
}