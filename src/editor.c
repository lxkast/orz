#include "row.h"

void insert_char(CFG* cfg, int c) {
    if (cfg->cy == cfg->num_rows) {
        add_row(cfg, "", 0);
    }
    row_char_insert(&cfg->trow[cfg->cy], c, cfg->cx++);
    cfg->last_cx = cfg->cx;
}