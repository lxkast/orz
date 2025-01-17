#include "cfg.h"
#include <stdlib.h>
#include <string.h>

#define TAB_SPACES 8

void add_row(CFG* cfg, char* s, int length) {
    cfg->trow = realloc(cfg->trow, (cfg->num_rows + 1) * sizeof(TEXT_ROW));
    int i = cfg->num_rows;
    cfg->trow[i].length = length;
    cfg->trow[i].text = malloc(length + 1);
    memcpy(cfg->trow[i].text, s, length);
    cfg->trow[i].text[length] = '\0';
    cfg->num_rows++;
}

int renderx_to_cx(TEXT_ROW* row, int cx) {
    int renderx = 0;
    for (int i = 0; i < cx; ++i) {
        if (row->text[i] == '\t') {
            // TAB_SPACES is how much to increment, (we -1 because we add after)
            // (renderx % TAB_SPACES) is how far right we are from prev indent
            renderx += (TAB_SPACES - 1) - (renderx % TAB_SPACES);
        }
        renderx++;
    }
    return renderx;
}