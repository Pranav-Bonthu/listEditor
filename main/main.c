#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include "buffer.h"

int main(){
    BufferLines buffer;
    initial_buffer(&buffer);
    load_file(&buffer, "list.txt");
    print_buffer(&buffer);
    free_buffer(&buffer);
 
    return 0;
}