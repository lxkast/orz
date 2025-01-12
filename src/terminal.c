#include "terminal.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

struct termios prev_terminal_attributes;

int get_window_size(int* rows, int* cols) {
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
        return -1;
    else {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
        return 0;
    }
}

char read_key() {
    char c;
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
        kill_self("read");
    return c;
}

int get_cursor_pos(int* row, int* col) {
    char buffer[32];
    unsigned int i = 0;
    // query the terminal for cursor pos
    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
        return -1;
    while (i < sizeof(buffer) - 1) {
        if (read(STDIN_FILENO, &buffer[i], 1) != 1) break;
        if (buffer[i] == 'R')
            break;
        i++;
    }
    buffer[i] = '\0';
    // the response is an escape sequence
    if (buffer[0] != '\x1b' || buffer[1] != '[') 
        return -1;
    if (sscanf(&buffer[2], "%d;%d", row, col) != 2)
        return -1;
    return 0;
}

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