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
        printf("Error opening file for writing");
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
    clear(); // clears the screen before printing
    
    for (int i = 0; i < buffer->count; i++){
         mvprintw(i, 0, "%s", buffer->lines[i]); 
         // printf does nto work with ncurses, so mvprintw is used 
    }
}

void insert_char(char *line, char c, int position){
    size_t len = strlen(line);
    for (size_t i = len; i >= position; i--){
        line[i + 1] = line[i]; // shift characters to the right
    }
    line[position] = c; // insert the new character
}

void delete_char(char *line, int position){
    size_t len = strlen(line);
    for (size_t i = position; i < len; i++){
        line[i] = line[i + 1]; // shift characters to the left
    }
}

void edit_buffer(BufferLines *buffer){

    int cursor_row = 0;
    int cursor_col = 0;

    pthread_mutex_lock(&buffer->lock);
    while (1){

        print_buffer(buffer);
        move(cursor_row, cursor_col); // moves the cursor to the specified position

        refresh(); // refreshes the screen to update changes
        int c = getch(); // waits for key press, reutrns int of the pressed key
        //getch();

        if (c == 27){ // ESC key to exit editing mode
            break;
        }

        switch (c){
        case 259: // left arrow key
            cursor_row = cursor_row - 1;
            if (cursor_row < 0){cursor_row = 0;}
            break;

        case 258: // right arrow key
            int line_len = strlen(buffer->lines[cursor_row]); 
            cursor_row = cursor_row + 1;
            if (cursor_row > line_len ){cursor_row = line_len;}            
            break;      
        case 260: // up arrow key
            cursor_col = cursor_col - 1;
            if (cursor_row == 0){cursor_row = 0;}
            break;
        case 261: // down arrow key
            cursor_col = cursor_col + 1;
            if (cursor_row == buffer->count - 1){cursor_row = buffer->count - 1;}
            break;
        case 127: // backspace key
            delete_char(buffer->lines[cursor_row], cursor_col - 1);
            cursor_col = cursor_col - 1;
            break;
        default:
            if (c >= 32 && c <= 126){ // printable characters
                insert_char(buffer->lines[cursor_row], (char)c, cursor_col);
                cursor_col = cursor_col + 1;
            }
            break;
        }
    }
        pthread_mutex_unlock(&buffer->lock);
}        
