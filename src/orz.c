#include "utils.h"
#include "terminal.h"
#include "output.h"
#include "input.h"
#include "cfg.h"
#include "fileio.h"

/*
          TODOS
          -----
    - disable ctrl-v terminal signal
    - resolve asprintf and getline compiler warning
    - smart vim-like scrolling
    - make scrolling left not awful
    - some sort of welcome message, maybe orz ascii art
*/

CFG cfg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void init() {
    enable_raw_terminal();
    if (get_window_size(&cfg.screen_rows, &cfg.screen_cols) == -1)
        kill_self("get window size");
}

int main(int argc, char* argv[]){
    init();
    if (argc > 1) {
        open_file(&cfg, argv[1]);
    }

    while (1) {
        refresh_screen(&cfg);
        process_key(&cfg); 
    }
    return 0;
}