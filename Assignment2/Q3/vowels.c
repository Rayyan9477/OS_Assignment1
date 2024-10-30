#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

#define MAX_BUF 1024

void display_instructions() {
    printf("Vowel Counting Program\n");
    printf("This program counts the number of vowels in a given string.\n");
    printf("Instructions:\n");
    printf("1. Ensure the named pipe 'vowel_pipe' exists.\n");
    printf("2. The program will read strings from the named pipe and count the vowels.\n");
}

int count_vowels(const char *str) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        char c = tolower(str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int main() {
    int fd;
    char buf[MAX_BUF];
    
    display_instructions();
    
    // Open named pipe
    fd = open("vowel_pipe", O_RDONLY);
    
    while (1) {
        // Clear buffer
        memset(buf, 0, MAX_BUF);
        
        // Read from pipe
        int bytes_read = read(fd, buf, MAX_BUF);
        if (bytes_read <= 0) {
            close(fd);
            fd = open("vowel_pipe", O_RDONLY);
            continue;
        }
        
        // Process and display result
        int vowel_count = count_vowels(buf);
        printf("The number of vowels in the entered string is: %d   in the string: %s\n" , vowel_count, buf);
    }
    
    close(fd);
    return 0;
}