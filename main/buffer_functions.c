#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include "buffer.h"



// giving initial values to the buffer
void initial_buffer(BufferLines *buffer){
    buffer -> count = 0; // "->" is used for dereferencing (changes the value the pointer points to)
    buffer -> capacity = 8;
    buffer -> lines = malloc(sizeof(char*) * buffer -> capacity); // capacity * size of char pointer
    pthread_mutex_init(&buffer -> lock, NULL); // initialize the mutex
}


// reallocated memory when new line are added to the buffer
void edit_buffer(BufferLines *buffer, const char *new_line){
    pthread_mutex_lock(&buffer -> lock); //lock the buffer for memory reallocation

    if (buffer -> count >= buffer -> capacity) {
        buffer -> capacity *= 2; // double the exsiting capacity 
        char **checker = realloc(buffer -> lines, sizeof(char*) * buffer -> capacity); // reallocate memory in a temporary variable, in case it fails
    

        if (checker == NULL){ // when the realloc fails, NULL will be returned
            pthread_mutex_unlock(&buffer -> lock); //unlock buffer
            printf("Memory allocation failed\n");
            return;
        }
        buffer -> lines = checker; // update the lines pointer to the new memory location using the temp vairable, since we know it succeeded
    }

    
    int new_memory = strlen(new_line) + 1; 
    buffer -> lines[buffer -> count] = malloc(new_memory); // allocates memory for the new line 
    // buffer is pointing to lines[index], which is a char*, so we are providing memory for the new line 
        
    if (buffer -> lines[buffer -> count] == NULL){ // when the realloc fails, NULL will be returned
        pthread_mutex_unlock(&buffer -> lock); //unlock buffer
        printf("Memory allocation for new line failed\n");
        return;
    }

    strcpy(buffer -> lines[buffer -> count], new_line); // copy the new line into the allocated memory
    buffer -> count = buffer -> count +1; //updates how many lines are taking up memory in the buffer
    pthread_mutex_unlock(&buffer -> lock); // unlock the buffer after editing
 }


//freeing buffer memory
void free_buffer(BufferLines *buffer){
    for(int i = 0; i < buffer -> count; i++){
        free(buffer -> lines[i]); // free each line
    }
    free(buffer -> lines); // free the array of line pointers
    pthread_mutex_destroy(&buffer -> lock); // destroy the mutex
    buffer = NULL;
}   





