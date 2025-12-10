#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include "buffer.h"

int main() {
    BufferLines buffer;

    initial_buffer(&buffer);
    load_file(&buffer, "list.txt");

    initscr(); // start ncurses mode
    cbreak();  // disable line buffering
    noecho();  // don't echo input characters
    keypad(stdscr, TRUE); // enable arrow keys

    print_buffer(&buffer);
    refresh(); // ADD THIS - display the buffer on screen

    edit_buffer(&buffer);
    endwin(); // end ncurses mode

    free_buffer(&buffer);

    return 0;
}