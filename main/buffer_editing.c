#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include "buffer.h"

const char *filename = "list.txt";


void load_file(BufferLines *buffer, const char *filename){
    FILE *fptr;
    char line[1024];

    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Error opening file for writing!");
        return ; // Indicate an error
    }

    while (fgets(line,sizeof(line),fptr) != NULL){
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n'){ // \n represents the end of a line. Single quotes for single character 
            line[len - 1] = '\0'; // \0 is the null character, ends the string 
        }
       edit_buffer_memory(buffer, line); 
    }
    fclose(fptr);
    return;
}

void print_buffer(BufferLines *buffer){
    pthread_mutex_lock(&buffer->lock);
    
    for (int i = 0; i < buffer->count; i++){
        printf("Line %d: %s\n", i + 1, buffer->lines[i]);
    }
    
    pthread_mutex_unlock(&buffer->lock);
}
