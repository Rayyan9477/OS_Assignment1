#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_BUF 1024

void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len/2; i++) {
        char temp = str[i];
        str[i] = str[len-1-i];
        str[len-1-i] = temp;
    }
}

int main() {
    int fd;
    char buf[MAX_BUF];
    char reversed[MAX_BUF];
    
    // Open named pipe
    fd = open("reverse_pipe", O_RDONLY);
    
    while (1) {
        // Clear buffers
        memset(buf, 0, MAX_BUF);
        memset(reversed, 0, MAX_BUF);
        
        // Read from pipe
        int bytes_read = read(fd, buf, MAX_BUF);
        if (bytes_read <= 0) {
            close(fd);
            fd = open("reverse_pipe", O_RDONLY);
            continue;
        }
        
        // Process and display result
        strcpy(reversed, buf);
        reverse_string(reversed);
        printf("Reversed string: %s\n", reversed);
    }
    
    close(fd);
    return 0;
}