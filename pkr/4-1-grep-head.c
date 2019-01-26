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

    pid_t grep_pid = fork();
    switch (grep_pid)
    {
    case -1:
        perror("fork");
        exit(9);

    case 0:
        close(pipe_fileno[0]);
        dup2(pipe_fileno[1], STDOUT_FILENO);

        argv[1] = "grep";
        if (-1 == execvp("grep", argv + 1))
        {
            perror("exec");
            exit(8);
        }
    }

    pid_t head_pid = fork();
    switch (head_pid)
    {
    case -1:
        perror("fork");
        exit(9);

    case 0:
        dup2(pipe_fileno[0], STDIN_FILENO);
        close(pipe_fileno[1]);

        if (-1 == execlp("head", "head", "-n", argv[1], NULL))
        {
            perror("exec");
            exit(8);
        }
    }

    close(pipe_fileno[0]);
    close(pipe_fileno[1]);

    waitpid(grep_pid, NULL, 0);

    int status;
    waitpid(head_pid, &status, 0);
    return WEXITSTATUS(status);
}