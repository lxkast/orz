#include "cfg.h"
#include <string.h>

void flip_mode(CFG* cfg) {
    cfg->insert_mode = !cfg->insert_mode;
}

void show_msg(CFG* cfg, char* msg) {
    int length = strlen(msg);
    if (length > 99)
        length = 99;
    memcpy(cfg->msg, msg, length);
    cfg->msg_time = time(NULL);
}