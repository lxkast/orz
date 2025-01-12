#include "terminal.h"
#include "input.h"
#include "utils.h"
#include "output.h"
#include <stdlib.h>

void process_key() {
    char c = read_key();
    switch (c) {
        case CTRL('w'):
            clear_screen();
            exit(0);
            break;
    }
}