#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "fileio.h"
#include "utils.h"
#include "cfg.h"

void open_file(CFG* cfg, char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        char* msg;
        asprintf(&msg, "Unable to open file %s", filename);
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