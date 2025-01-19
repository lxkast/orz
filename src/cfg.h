#pragma once
#include <time.h>

typedef struct TEXT_ROW{
    char* text;
    int length;
    // render stores the characters of a row that
    // will be printed to the terminal
    char* render;
    int render_length;
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
    char* filename;
    _Bool insert_mode;
    char msg[100];
    time_t msg_time;
} CFG;

void flip_mode(CFG*);

void show_msg(CFG*, char*);