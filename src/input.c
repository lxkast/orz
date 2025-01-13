#include "terminal.h"
#include "input.h"
#include "utils.h"
#include "output.h"
#include <stdlib.h>

void move_cursor(CFG* cfg, int key) {
    switch (key) {
        case ARROW_LEFT:
            if (cfg->cx > 0)
                cfg->cx--;
            break;
        case ARROW_DOWN:
            if (cfg->cy < cfg->screen_rows)
                cfg->cy++;
            break;
        case ARROW_UP:
            if (cfg->cy > 0)
                cfg->cy--;
            break;
        case ARROW_RIGHT:
            if (cfg->cx < cfg->screen_cols)
                cfg->cx++;
            break;
    }
}

void process_key(CFG* cfg) {
    int c = read_key();
    switch (c) {
        case CTRL('w'):
            clear_screen();
            exit(0);
            break;

        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            move_cursor(cfg, c);
            break;
    }
}