#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

#define MAX_BUF 1024

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
    
    // Open named pipe
    fd = open("capital_pipe", O_RDONLY);
    
    while (1) {
        // Clear buffer
        memset(buf, 0, MAX_BUF);
        
        // Read from pipe
        int bytes_read = read(fd, buf, MAX_BUF);
        if (bytes_read <= 0) {
            close(fd);
            fd = open("capital_pipe", O_RDONLY);
            continue;
        }
        
        // Process and display result
        int capital_count = count_capitals(buf);
        printf("Number of capital letters: %d in text: %s\n", capital_count, buf);
    }
    
    close(fd);
    return 0;
}