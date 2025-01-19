#include "cfg.h"

void flip_mode(CFG* cfg) {
    cfg->insert_mode = !cfg->insert_mode;
}