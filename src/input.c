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
        case 'i':
            flip_mode(cfg);
    }
}

void process_insert_key(CFG* cfg, int c) {
    switch (c) {
        // ENTER
        case '\r':
            break;
        case BACKSPACE:
        case CTRL('h'):
            break;
        default:
            insert_char(cfg, c);
    }
}

void process_key(CFG* cfg) {
    int c = read_key();
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