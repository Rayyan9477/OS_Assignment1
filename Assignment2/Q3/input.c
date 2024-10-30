#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_BUF 1024

void display_instructions() {
    printf("Input Program\n");
    printf("This program takes input from the user and passes it to other programs via named pipes.\n");
    printf("Instructions:\n");
    printf("1. Enter a string when prompted.\n");
    printf("2. The string will be sent to other programs for processing.\n");
    printf("3. Type 'exit' to quit the program.\n");
}

int main() {
    int fd_vowels, fd_reverse, fd_capitals;
    char input[MAX_BUF];
    
    // Create named pipes if they don't exist
    mkfifo("vowel_pipe", 0666);
    mkfifo("reverse_pipe", 0666);
    mkfifo("capital_pipe", 0666);
    
    display_instructions();
    printf("Enter text (type 'exit' to quit):\n");
    
    while (1) {
        // Get input from user
        fgets(input, MAX_BUF, stdin);
        input[strcspn(input, "\n")] = 0;  // Remove newline
        
        if (strcmp(input, "exit") == 0) {
            break;
        }
        
        // Open pipes and write data
        fd_vowels = open("vowel_pipe", O_WRONLY);
        fd_reverse = open("reverse_pipe", O_WRONLY);
        fd_capitals = open("capital_pipe", O_WRONLY);
        
        write(fd_vowels, input, strlen(input));
        write(fd_reverse, input, strlen(input));
        write(fd_capitals, input, strlen(input));
        
        close(fd_vowels);
        close(fd_reverse);
        close(fd_capitals);
    }
    
    // Clean up pipes
    unlink("vowel_pipe");
    unlink("reverse_pipe");
    unlink("capital_pipe");
    
    return 0;
}