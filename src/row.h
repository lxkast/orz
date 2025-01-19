#pragma once
#include "cfg.h"

void fill_render_row(TEXT_ROW*);

void add_row(CFG*, char*, int);
void insert_row(CFG*, char*, int length, int index);
void delete_row(CFG* cfg, int);
void append_to_row(TEXT_ROW*, char*, int);
void row_char_insert(TEXT_ROW*, int c, int index);
void row_char_delete(TEXT_ROW*, int index);

int renderx_to_cx(TEXT_ROW*, int);