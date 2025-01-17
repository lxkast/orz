#include "row.h"
#include <stdlib.h>
#include <string.h>

#define TAB_SPACES 4

void fill_render_row(TEXT_ROW* row) {
    // count number of tabs so we can malloc enough space for the rendered line
    int tabs = 0;
    for (int i = 0; i < row->length; ++i) {
        if (row->text[i] == '\t') {
            tabs++;
        }
    }

    free(row->render);
    row->render = malloc(tabs * (TAB_SPACES - 1) + row->length + 1);
    int render_index = 0;
    for (int i = 0; i < row->length; ++i) {
        if (row->text[i] == '\t') {
            // do this before loop to ensure we end up at next tab space
            row->render[render_index++] = ' ';
            while (render_index % TAB_SPACES != 0) {
                row->render[render_index++] = ' ';
            }
        }
        else {
            row->render[render_index++] = row->text[i];
        }
    }
    row->render[render_index] = '\0';
    row->render_length = render_index;
}

void add_row(CFG* cfg, char* s, int length) {
    cfg->trow = realloc(cfg->trow, (cfg->num_rows + 1) * sizeof(TEXT_ROW));
    int i = cfg->num_rows;
    cfg->trow[i].length = length;
    cfg->trow[i].text = malloc(length + 1);
    memcpy(cfg->trow[i].text, s, length);
    cfg->trow[i].text[length] = '\0';
    cfg->trow[i].render_length = 0;
    cfg->trow[i].render = NULL;
    fill_render_row(&cfg->trow[i]);
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