#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_BUF 1024

void display_instructions() {
    printf("String Reversal Program\n");
    printf("This program reads a string from a named pipe and reverses it.\n");
    printf("Instructions:\n");
    printf("1. Ensure the named pipe 'reverse_pipe' exists.\n");
    printf("2. The program will read strings from the named pipe and reverse them.\n");
}

void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int main() {
    int fd;
    char buf[MAX_BUF];
    char reversed[MAX_BUF];
    
    display_instructions();
    
    fd = open("reverse_pipe", O_RDONLY);
    
    while (1) {
        memset(buf, 0, MAX_BUF);
        memset(reversed, 0, MAX_BUF);
        
        int bytes_read = read(fd, buf, MAX_BUF);
        if (bytes_read <= 0) {
            close(fd);
            fd = open("reverse_pipe", O_RDONLY);
            continue;
        }
        
        strcpy(reversed, buf);
        reverse_string(reversed);
        printf("Reversed string: %s\n", reversed);
    }
    
    close(fd);
    return 0;
}