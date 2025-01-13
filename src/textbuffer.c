#include "textbuffer.h"
#include <stdlib.h>
#include <string.h>

void tb_append(TEXTBUFFER* tb, const char* s, int len) {
    char* new = realloc(tb->buffer, tb->len + len);
    if (new == NULL)
        return;
    
    memcpy(&new[tb->len], s, len);
    tb->buffer = new;
    tb->len += len;
}

void tb_free(TEXTBUFFER* tb) {
    free(tb->buffer);
}