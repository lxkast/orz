#include "cfg.h"
#include <stdlib.h>
#include <string.h>

void add_row(CFG* cfg, char* s, int length) {
    cfg->trow = realloc(cfg->trow, (cfg->num_rows + 1) * sizeof(TEXT_ROW));
    int i = cfg->num_rows;
    cfg->trow[i].length = length;
    cfg->trow[i].text = malloc(length + 1);
    memcpy(cfg->trow[i].text, s, length);
    cfg->trow[i].text[length] = '\0';
    cfg->num_rows++;
}