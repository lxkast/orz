#include "output.h"
#include "utils.h"
#include "textbuffer.h"
#include "terminal.h"
#include "row.h"
#include "cfg.h"
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

int draw_msg(CFG* cfg, TEXTBUFFER* tb) {
    // bold
    tb_append(tb, "\x1b[1m", 4);
    int length = strlen(cfg->msg);
    if (length > cfg->screen_cols) {
        length = cfg->screen_cols;
    }
    tb_append(tb, cfg->msg, length);
    // un-bold
    tb_append(tb, "\x1b[m", 3);
    return length;
}

void draw_bottom_row(CFG* cfg, TEXTBUFFER* tb) {
    int length;
    if (time(NULL) - cfg->msg_time < 2) {
        length = draw_msg(cfg, tb);
    }
    else {
        if (cfg->insert_mode) {
            // bold
            tb_append(tb, "\x1b[1m", 4);
            char* insert_text = "[-- INSERT --]";
            length = strlen(insert_text);
            if (length > cfg->screen_cols) {
                length = cfg->screen_cols;
            }
            tb_append(tb, insert_text, length);

            // unbold
            tb_append(tb, "\x1b[m", 3);
        }
        else {
            // low intensity text
            tb_append(tb, "\x1b[2m", 4);
            // print filename
            char filename_buffer[100];
            length = snprintf(
                filename_buffer, 
                sizeof(filename_buffer), 
                "%.20s",
                cfg->filename ? cfg->filename : "[untitled]");
            if (length > cfg->screen_cols) {
                length = cfg->screen_cols;
            }
            tb_append(tb, filename_buffer, length);
        }
    }


    // low intensity text
    tb_append(tb, "\x1b[2m", 4);

    // print cursor position
    char cursor_pos_buffer[100];
    int cursor_length = snprintf(cursor_pos_buffer, 
        sizeof(cursor_pos_buffer), 
        "%d;%d",
        cfg->cy + 1, cfg->cx + 1);

    for(; length < cfg->screen_cols; ++length) {
        if (cursor_length == cfg->screen_cols - length) {
            tb_append(tb, cursor_pos_buffer, cursor_length);
            break;
        }
        tb_append(tb, " ", 1);
    }

    // un-invert
    tb_append(tb, "\x1b[m", 3);
}

void draw_rows(CFG* cfg, TEXTBUFFER* tb) {
    // move cursor to top left and erase line
    tb_append(tb, "\x1b[H", 3);
    tb_append(tb, "\x1b[K", 3);

    for (int i = 0; i < cfg->screen_rows; ++i) {
        int row_index = i + cfg->view_row_offset;
        if (row_index < cfg->num_rows) {
            int length = cfg->trow[row_index].render_length - cfg->view_col_offset;
            if (length < 0)
                length = 0;
            if (length > cfg->screen_cols)
                length = cfg->screen_cols;
            tb_append(tb, &cfg->trow[row_index].render[cfg->view_col_offset], length);
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

void scroll(CFG* cfg) {
    cfg->renderx = 0;
    if (cfg->cy < cfg->num_rows) {
        cfg->renderx = renderx_to_cx(&cfg->trow[cfg->cy], cfg->cx);
    }

    if (cfg->cy < cfg->view_row_offset) {
        cfg->view_row_offset = cfg->cy;
    }
    if (cfg->cy >= cfg->view_row_offset + cfg->screen_rows) {
        cfg->view_row_offset = cfg->cy - cfg->screen_rows + 1;
    }
    if (cfg->renderx < cfg->view_col_offset) {
        cfg->view_col_offset = cfg->renderx;
    }
    if (cfg->renderx >= cfg->view_col_offset + cfg->screen_cols) {
        cfg->view_col_offset = cfg->renderx - cfg->screen_cols + 1;
    }
}

void refresh_screen(CFG* cfg) {
    TEXTBUFFER tb = {NULL, 0};

    scroll(cfg);

    // hide cursor
    tb_append(&tb, "\x1b[?25l", 6);

    if (get_window_size(&cfg->screen_rows, &cfg->screen_cols) == -1)
        kill_self("get window size");

    draw_rows(cfg, &tb);
    
    // set cursor pos
    char buffer[32];
    snprintf(buffer, 
        sizeof(buffer), 
        "\x1b[%d;%dH", 
        cfg->cy - cfg->view_row_offset + 1,
        cfg->renderx - cfg->view_col_offset + 1);
    tb_append(&tb, buffer, strlen(buffer));

    // show cursor
    tb_append(&tb, "\x1b[?25h", 6);

    write(STDOUT_FILENO, tb.buffer, tb.len);
    tb_free(&tb);
}
