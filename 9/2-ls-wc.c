#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, const char* const* argv)
{
    int pipe_fileno[2];
    if (-1 == pipe(pipe_fileno))
    {
        perror(argv[0]);
        return 1;
    }

    pid_t ls_pid = fork();
    switch (ls_pid)
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

        if (-1 == execlp("ls", "ls", NULL))
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

    waitpid(ls_pid, NULL, 0);

    int status;
    if (-1 == waitpid(wc_pid, &status, 0))
        perror(argv[0]);

    return WEXITSTATUS(status);
}