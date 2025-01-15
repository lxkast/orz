#pragma once

typedef struct TEXT_ROW{
    char* text;
    int length;
} TEXT_ROW;

typedef struct CFG {
    int cx, cy;
    int screen_rows, screen_cols;
    int num_rows;
    // dynamically allocated array of text rows
    TEXT_ROW* trow;
} CFG;

void add_row(CFG*, char*, int);