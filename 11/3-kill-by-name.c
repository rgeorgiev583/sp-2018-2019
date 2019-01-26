#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int pipe_fileno[2];
    if (-1 == pipe(pipe_fileno))
    {
        perror("pipe");
        exit(12);
    }

    pid_t pidof_pid = fork();
    switch (pidof_pid)
    {
    case -1:
        perror("fork");
        exit(9);

    case 0:
        close(pipe_fileno[0]);
        dup2(pipe_fileno[1], STDOUT_FILENO);

        argv[0] = "pidof";
        if (-1 == execvp("pidof", argv))
        {
            perror("exec");
            exit(8);
        }
    }

    pid_t xargs_pid = fork();
    switch (xargs_pid)
    {
    case -1:
        perror("fork");
        exit(9);

    case 0:
        dup2(pipe_fileno[0], STDIN_FILENO);
        close(pipe_fileno[1]);

        if (-1 == execlp("xargs", "xargs", "kill", NULL))
        {
            perror("exec");
            exit(8);
        }
    }

    close(pipe_fileno[0]);
    close(pipe_fileno[1]);

    waitpid(pidof_pid, NULL, 0);
    waitpid(xargs_pid, NULL, 0);

    return 0;
}