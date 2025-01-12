#include "utils.h"
#include <stdio.h>

void kill_self(const char* s) {
    printf("%s", s);
    exit(1);
}