#include "row.h"

void delete_key(CFG* cfg);

void insert_char(CFG* cfg, int c) {
    if (cfg->cy == cfg->num_rows) {
        add_row(cfg, "", 0);
    }
    row_char_insert(&cfg->trow[cfg->cy], c, cfg->cx++);
    cfg->last_cx = cfg->cx;
}

void delete_char(CFG* cfg) {
    if (cfg->cy == cfg->num_rows)
        return;
    if (cfg->cx == 0 && cfg->cy == 0)
        return;
    TEXT_ROW* row = &cfg->trow[cfg->cy];
    if (cfg->cx > 0) {
        row_char_delete(row, cfg->cx - 1);
        cfg->cx--;
        cfg->last_cx = cfg->cx;
    }
    else {
        cfg->cx = cfg->trow[cfg->cy - 1].length;
        append_to_row(&cfg->trow[cfg->cy - 1], row->text, row->length);
        delete_row(cfg, cfg->cy);
        cfg->cy--;
    }
}