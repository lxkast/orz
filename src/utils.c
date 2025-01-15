#include "utils.h"
#include "output.h"
#include <stdio.h>
#include <stdlib.h>

void kill_self(const char* s) {
    clear_screen();

    printf("%s\r\n", s);
    exit(1);
}