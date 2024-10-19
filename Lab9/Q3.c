#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
    int fd = open("error_log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    if (dup2(fd, STDERR_FILENO) < 0)
    {
        perror("dup2");
        close(fd);
        return 1;
    }
    fprintf(stderr, "This is an error message!\n");
    close(fd);
    return 0;
}