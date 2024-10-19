#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd = open("input.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    if (dup2(fd, STDIN_FILENO) < 0)
    {
        perror("dup2");
        close(fd);
        return 1;
    }
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        printf("Data from file: %s\n", buffer);
    }
    else
    {
        perror("fgets");
    }
    // Clean up
    close(fd);
    return 0;
}