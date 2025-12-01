#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>


// setting up the buffer
//the buffer will be edited by the users, once editing is complete,
// the buffer will be written in list.txt
//This will help to avoid errors while multiple users are editing the file
typedef struct {
    char **lines; // a pointer to an array of line pointers 
    int count; // the amount of lines in the buffer
    int capacity;// the amount of lines pointers allocated
    pthread_mutex_t lock; //only one thread can edit the buffer 
} BufferLines;


void initial_buffer(BufferLines *buffer){
    buffer -> count = 0; // "->" is used for dereferencing (changing the value the pointer points to   )
    buffer -> capacity = 8;
    buffer -> lines = malloc(sizeof(char*) * buffer -> capacity); // capacity * size of char pointer
    pthread_mutex_init(&buffer -> lock, NULL); // initialize the mutex
}

