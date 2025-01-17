#pragma once

typedef struct TEXT_ROW{
    char* text;
    int length;
} TEXT_ROW;

typedef struct CFG {
    int cx, cy;
    int last_cx;
    int renderx;
    int screen_rows, screen_cols;
    int num_rows;
    // dynamically allocated array of text rows
    TEXT_ROW* trow;
    int view_row_offset;
    int view_col_offset;
} CFG;

void add_row(CFG*, char*, int);

int renderx_to_cx(TEXT_ROW*, int);