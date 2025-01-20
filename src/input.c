#include "terminal.h"
#include "input.h"
#include "utils.h"
#include "output.h"
#include "editor.h"
#include "fileio.h"
#include <stdlib.h>

void move_cursor(CFG* cfg, int key) {
    TEXT_ROW* row = (cfg->cy >= cfg->num_rows) ? NULL : &cfg->trow[cfg->cy];
    switch (key) {
        case ARROW_LEFT:
            if (cfg->cx > 0) {
                cfg->cx--;
            }
            else if (cfg->cy > 0) {
                cfg->cy--;
                cfg->cx = cfg->trow[cfg->cy].length;
            }
            cfg->last_cx = cfg->cx;
            break;
        case ARROW_DOWN:
            if (cfg->cy == cfg->num_rows - 1) {
                cfg->cx = cfg->trow[cfg->cy].length;
                cfg->last_cx = cfg->cx;
            }
            if (cfg->cy < cfg->num_rows - 1) {
                cfg->cy++;
            }
            break;
        case ARROW_UP:
            if (cfg->cy > 0)
                cfg->cy--;
            break;
        case ARROW_RIGHT:
            if (row && row->length > cfg->cx) {
                cfg->cx++;
                cfg->last_cx = cfg->cx;
            }
            break;
    }

    row = (cfg->cy >= cfg->num_rows) ? NULL : &cfg->trow[cfg->cy];
    int row_length = row ? row->length : 0;
    cfg->cx = cfg->last_cx;
    if (cfg->cx > row_length) {
        cfg->cx= row_length;
    }
}

void process_non_insert_key(CFG* cfg, int c) {
    switch (c) {
        case 'h':
            move_cursor(cfg, ARROW_LEFT);
            break;
        case 'j':
            move_cursor(cfg, ARROW_DOWN);
            break;
        case 'k':
            move_cursor(cfg, ARROW_UP);
            break;
        case 'l':
            move_cursor(cfg, ARROW_RIGHT);
            break;
        case 'o':
            cfg->cx = cfg->trow[cfg->cy].length;
            insert_line(cfg);
            flip_mode(cfg);
            break;
        case 'O':
            cfg->cx = 0;
            insert_line(cfg);
            cfg->cy--;
            flip_mode(cfg);
            break;
        case '0':
            cfg->cx = 0;
            cfg->last_cx = 0;
            break;
        case '$':
            if (cfg->num_rows != 0) {
                if (cfg->trow[cfg->cy].length <= 1)
                    break;
                cfg->cx = cfg->trow[cfg->cy].length - 1;
                cfg->last_cx = cfg->cx;
            }
            break;
        case 'x':
            move_cursor(cfg, ARROW_RIGHT);
            delete_char(cfg);
            if (cfg->cx == cfg->trow[cfg->cy].length) {
                move_cursor(cfg, ARROW_LEFT);
            }
            break;
        case 'i':
            flip_mode(cfg);
            break;
        case 'a':
            move_cursor(cfg, ARROW_RIGHT);
            flip_mode(cfg);
            break;
    }
}


void process_insert_key(CFG* cfg, int c) {
    switch (c) {
        case ESC:
            flip_mode(cfg);
            if (cfg->cx != 0) {
                move_cursor(cfg, ARROW_LEFT);
            }
            break;
        // ENTER
        case '\r':
            insert_line(cfg);
            break;
        case BACKSPACE:
        case CTRL('h'):
            delete_char(cfg);
            break;
        default:
            insert_char(cfg, c);
    }
}

void process_key(CFG* cfg) {
    int c = read_key(cfg);
    switch (c) {
        case CTRL('w'):
            clear_screen();
            exit(0);
            return;
        case CTRL('s'):
            save_to_disk(cfg);
            return;

        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            move_cursor(cfg, c);
            return;
    }

    if (cfg->insert_mode) {
        process_insert_key(cfg, c);
    }
    else {
        process_non_insert_key(cfg, c);
    }
}
