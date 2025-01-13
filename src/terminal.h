enum keys {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DELETE_KEY
};

void enable_raw_terminal();
int read_key();
int get_window_size(int*, int*);