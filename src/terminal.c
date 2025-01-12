#include "terminal.h"
#include "utils.h"
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios prev_terminal_attributes;

void disable_raw_terminal() {
    // TCSAFLUSH sets terminal attributes after the buffer is flushed.
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &prev_terminal_attributes) == -1)
        kill_self("tcsetattr");
}

void enable_raw_terminal() {
    if (tcgetattr(STDIN_FILENO, &prev_terminal_attributes) == -1)
        kill_self("tcgetattr");
    atexit(disable_raw_terminal);

    struct termios terminal_attributes = prev_terminal_attributes;
    // disable ctrl-s and ctrl-q signals
    // and disable ctrl-m signals.
    terminal_attributes.c_iflag &= ~(IXON | ICRNL);
    // disable printing typed characters to terminal
    // and disable canonical mode (reading input line by line)
    // and disable ctrl-c and ctrl-z signals
    // and disable ctrl-v signals.
    terminal_attributes.c_lflag &= ~(ECHO | ICANON | ISIG | ISIG);
    // disable output post-processing
    terminal_attributes.c_oflag &= ~(OPOST);
    terminal_attributes.c_cc[VMIN] = 0;
    terminal_attributes.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal_attributes);
}