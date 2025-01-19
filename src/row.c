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

void row_char_delete(TEXT_ROW* row, int index) {
    if (index < 0 || index > row->length)
        return;
    
    memmove(&row->text[index], &row->text[index + 1], row->length - index);
    row->length--;
    fill_render_row(row);
}

void row_char_insert(TEXT_ROW* row, int c, int index) {
    if (index < 0)
        return;
    if (index > row->length)
        index = row->length;
    row->text = realloc(row->text, row->length + 2);
    // make space to insert new character
    memmove(&row->text[index + 1], &row->text[index], row->length - index + 1);
    row->length++;
    row->text[index] = c;
    fill_render_row(row);
}

void insert_row(CFG* cfg, char* s, int length, int i) {
    if (i < 0 || i > cfg->num_rows)
        return;
    cfg->trow = realloc(cfg->trow, (cfg->num_rows + 1) * sizeof(TEXT_ROW));
    memmove(&cfg->trow[i + 1], &cfg->trow[i], sizeof(TEXT_ROW) * (cfg->num_rows - i));
    cfg->trow[i].length = length;
    cfg->trow[i].text = malloc(length + 1);
    memcpy(cfg->trow[i].text, s, length);
    cfg->trow[i].text[length] = '\0';
    cfg->trow[i].render_length = 0;
    cfg->trow[i].render = NULL;
    fill_render_row(&cfg->trow[i]);
    cfg->num_rows++;
}

void add_row(CFG* cfg, char* s, int length) {
    insert_row(cfg, s, length, cfg->num_rows);
}

void append_to_row(TEXT_ROW* row, char* s, int length) {
    row->text = realloc(row->text, row->length + length + 1);
    memcpy(&row->text[row->length], s, length);
    row->length += length;
    row->text[row->length] = '\0';
    fill_render_row(row);
}

void delete_row(CFG* cfg, int row_num) {
    if (row_num == 0 || row_num >= cfg->num_rows)
        return;
    free(cfg->trow[row_num].render);
    free(cfg->trow[row_num].text);
    memmove(&cfg->trow[row_num], 
            &cfg->trow[row_num + 1],  
            sizeof(TEXT_ROW) * (cfg->num_rows - row_num - 1));
    cfg->num_rows--;
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
