#pragma once

typedef struct TEXTBUFFER {
  char* buffer;
  int len;
} TEXTBUFFER;

void tb_append(TEXTBUFFER*, const char*, int);
void tb_free(TEXTBUFFER* tb);