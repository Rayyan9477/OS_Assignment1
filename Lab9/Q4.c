#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
int main()
{
    int pipe_fd[2];
    pid_t pid;
    char buffer[100];
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(1);
    }
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        close(pipe_fd[0]);               // Close the unused read end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the pipe's
            printf("Message from child\n");
        close(pipe_fd[1]);
        exit(0);
    }
    else
    {
        close(pipe_fd[1]); // Close the unused write end of the pipe
        read(pipe_fd[0], buffer, sizeof(buffer));
        printf("Parent received: %s", buffer);
        close(pipe_fd[0]);
        wait(NULL);
    }
    return 0;
}