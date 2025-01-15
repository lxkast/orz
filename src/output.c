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

void draw_bottom_row(CFG* cfg, TEXTBUFFER* tb) {
    char* pos_text;
    asprintf(&pos_text, "%d;%d", cfg->cy + 1, cfg->cx + 1);
    int pos_length = strlen(pos_text);

    if (cfg->screen_cols > pos_length + 3) {
        // not a fan of this, might change later
        char* line = malloc(cfg->screen_cols);
        for (int i = 0; i < cfg->screen_cols - pos_length - 2; ++i) {
            line[i] = ' ';
        }
        memcpy(&line[cfg->screen_cols - pos_length - 2], pos_text, strlen(pos_text));
        line[cfg->screen_cols + pos_length] = '\0';
        tb_append(tb, line, strlen(line));
        free(line);
    }
}

void draw_rows(CFG* cfg, TEXTBUFFER* tb) {
    write(STDOUT_FILENO, "\x1b[H", 3);
    tb_append(tb, "\x1b[K", 3);
    for (int i = 0; i < cfg->screen_rows - 1; ++i) {
        if (i < cfg->num_rows) {
            if (cfg->trow[i].length > cfg->screen_cols)
                tb_append(tb, cfg->trow[i].text, cfg->screen_cols);
            else
                tb_append(tb, cfg->trow[i].text, cfg->trow[i].length);
            tb_append(tb, "\r\n", 2);
        }
        else {
            tb_append(tb, "~\r\n", 3);
        }
            // erase everything right of cursor on current line
            tb_append(tb, "\x1b[K", 3);
    }
    draw_bottom_row(cfg, tb);
}

void refresh_screen(CFG* cfg) {
    TEXTBUFFER tb = {NULL, 0};
    // hide cursor
    tb_append(&tb, "\x1b[?25l", 6);

    draw_rows(cfg, &tb);
    
    // set cursor pos
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "\x1b[%d;%dH", cfg->cy + 1, cfg->cx + 1);
    tb_append(&tb, buffer, strlen(buffer));

    // show cursor
    tb_append(&tb, "\x1b[?25h", 6);
    write(STDOUT_FILENO, tb.buffer, tb.len);
    tb_free(&tb);
}