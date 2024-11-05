#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

#define MAX_BUF 1024

void display_instructions() {
    printf("Capital Letters Counting Program\n");
    printf("This program counts the number of capital letters in a given string.\n");
    printf("Instructions:\n");
    printf("1. Ensure the named pipe 'capital_pipe' exists.\n");
    printf("2. The program will read strings from the named pipe and count the capital letters.\n");
}

int count_capitals(const char *str) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        if (isupper(str[i])) {
            count++;
        }
    }
    return count;
}

int main() {
    int fd;
    char buf[MAX_BUF];
    
    display_instructions();
    
    fd = open("capital_pipe", O_RDONLY);
    
    while (1) {
        memset(buf, 0, MAX_BUF);
        
        int bytes_read = read(fd, buf, MAX_BUF);
        if (bytes_read <= 0) {
            close(fd);
            fd = open("capital_pipe", O_RDONLY);
            continue;
        }
        
        int capital_count = count_capitals(buf);
        printf("Number of capital letters: %d in text: %s\n", capital_count, buf);
    }
    
    close(fd);
    return 0;
}