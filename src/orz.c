#include "utils.h"
#include "terminal.h"
#include "output.h"
#include "input.h"
#include "cfg.h"

#define version "0.0.1"

CFG cfg = {0, 0, 0, 0};

void init() {
    enable_raw_terminal();
    if (get_window_size(&cfg.screen_rows, &cfg.screen_cols) == -1)
        kill_self("get window size");
}

int main(){
    init();

    while (1) {
        refresh_screen(&cfg);
        process_key(&cfg); 
    }
    return 0;
}