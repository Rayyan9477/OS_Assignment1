#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    int fd_dup = dup(fd);
    if (fd_dup < 0)
    {
        perror("dup");
        close(fd);
        return 1;
    }
    if (dup2(fd_dup, STDOUT_FILENO) < 0)
    {
        perror("dup2");
        close(fd_dup);
        close(fd);
        return 1;
    }
    printf("Hello world\n");
    close(fd_dup);
    close(fd);
    return 0;
}