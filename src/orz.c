#include "utils.h"
#include "terminal.h"
#include "output.h"
#include "input.h"
#include "cfg.h"
#include "fileio.h"

/*
                       TODOS

    - write from no file
    - prevent scrolling completely like normal text editors
    - show if file modified
    - make a function that updates cx and last_cx
    - resolve compiler warnings
    - smart vim-like scrolling
    - make scrolling left not awful
    - some sort of welcome message, maybe orz ascii art
    - undo stack

*/

CFG cfg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void init() {
    cfg.msg[0] = '\0';
    cfg.msg_time = 0;
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