#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, const char** argv)
{
    int pipe_fileno[2];
    if (-1 == pipe(pipe_fileno))
    {
        perror(argv[0]);
        return 1;
    }

    pid_t who_pid = fork();
    switch (who_pid)
    {
    case -1:
        perror(argv[0]);
        return 2;

    case 0:
        if (-1 == close(pipe_fileno[0]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == dup2(pipe_fileno[1], STDOUT_FILENO))
        {
            perror(argv[0]);
            return 4;
        }

        argv[0] = "grep";
        if (-1 == execvp("grep", (char* const*)argv))
        {
            perror(argv[0]);
            return 5;
        }
    }

    pid_t wc_pid = fork();
    switch (wc_pid)
    {
    case -1:
        perror(argv[0]);
        return 2;

    case 0:
        if (-1 == dup2(pipe_fileno[0], STDIN_FILENO))
        {
            perror(argv[0]);
            return 4;
        }

        if (-1 == close(pipe_fileno[1]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == execlp("wc", "wc", "-l", NULL))
        {
            perror(argv[0]);
            return 5;
        }
    }

    if (-1 == close(pipe_fileno[0]))
    {
        perror(argv[0]);
        return 3;
    }

    if (-1 == close(pipe_fileno[1]))
    {
        perror(argv[0]);
        return 3;
    }

    waitpid(who_pid, NULL, 0);

    int status;
    if (-1 == waitpid(wc_pid, &status, 0))
        perror(argv[0]);

    return WEXITSTATUS(status);
}