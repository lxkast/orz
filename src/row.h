#pragma once
#include "cfg.h"

void add_row(CFG*, char*, int);
void row_char_insert(TEXT_ROW*, int c, int index);

int renderx_to_cx(TEXT_ROW*, int);