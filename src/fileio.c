#include "fileio.h"
#include "utils.h"
#include "cfg.h"
#include "row.h"
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

char* export_string(CFG* cfg, int* length) {
    int total_length;
    for (int i = 0; i < cfg->num_rows; ++i) {
        total_length += cfg->trow[i].length + 1; // +1 for \n
    }
    *length = total_length;

    char* buffer = malloc(total_length);
    char* next_space = buffer;
    for (int i = 0; i < cfg->num_rows; ++i) {
        memcpy(next_space, cfg->trow[i].text, cfg->trow[i].length);
        next_space += cfg->trow[i].length;
        *next_space = '\n';
        next_space++;
    }

    // caller better free this!
    return buffer;
}

void save_to_disk(CFG* cfg) {
    // in the future, write to temp file first then rename

    if (cfg->filename == NULL)
        return;

    int length;
    char* buffer = export_string(cfg, &length);
    int file_descriptor = open(cfg->filename, O_RDWR | O_CREAT, 0644);
    if (file_descriptor != -1) {
        if (ftruncate(file_descriptor, length)!= -1) {
            write(file_descriptor, buffer, length);
            close(file_descriptor);
            free(buffer);
            return;
        }

        close(file_descriptor);
    }
    free(buffer);
}

void open_file(CFG* cfg, char* filename) {
    cfg->filename = malloc(strlen(filename) + 1);
    memcpy(cfg->filename, filename, strlen(filename) + 1);
    FILE* f = fopen(filename, "r");
    if (!f) {
        char* msg;
        if (asprintf(&msg, "Unable to open file %s", filename) == -1)
            kill_self("asprintf");
        kill_self(msg);
    }

    char* line = NULL;
    size_t linecap = 0;
    ssize_t length;
    while((length = getline(&line, &linecap, f)) != -1) {
        while (length > 0 && (line[length - 1] == '\n' || line[length - 1] == '\r')) {
            length--;
        }
        add_row(cfg, line, length);
    }
    
    free(line);
    fclose(f);
}