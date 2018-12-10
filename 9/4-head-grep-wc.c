#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 2

int main(int argc, const char** argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int pipe_head_grep_fileno[2];
    if (-1 == pipe(pipe_head_grep_fileno))
    {
        perror(argv[0]);
        return 1;
    }

    pid_t head_pid = fork();
    switch (head_pid)
    {
    case -1:
        perror(argv[0]);
        return 2;

    case 0:
        if (-1 == close(pipe_head_grep_fileno[0]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == dup2(pipe_head_grep_fileno[1], STDOUT_FILENO))
        {
            perror(argv[0]);
            return 4;
        }

        argv[0] = "head";
        argv[2] = argv[1];
        argv[1] = "-n";
        if (-1 == execvp("head", (char* const*)argv))
        {
            perror(argv[0]);
            return 5;
        }
    }

    int pipe_grep_wc_fileno[2];
    if (-1 == pipe(pipe_grep_wc_fileno))
    {
        perror(argv[0]);
        return 1;
    }

    pid_t grep_pid = fork();
    switch (grep_pid)
    {
    case -1:
        perror(argv[0]);
        return 2;

    case 0:
        if (-1 == dup2(pipe_head_grep_fileno[0], STDIN_FILENO))
        {
            perror(argv[0]);
            return 4;
        }

        if (-1 == close(pipe_head_grep_fileno[1]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == close(pipe_grep_wc_fileno[0]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == dup2(pipe_grep_wc_fileno[1], STDOUT_FILENO))
        {
            perror(argv[0]);
            return 4;
        }

        if (-1 == execlp("grep", "grep", argv[2], NULL))
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
        if (-1 == close(pipe_head_grep_fileno[0]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == close(pipe_head_grep_fileno[1]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == dup2(pipe_grep_wc_fileno[0], STDIN_FILENO))
        {
            perror(argv[0]);
            return 4;
        }

        if (-1 == close(pipe_grep_wc_fileno[1]))
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

    if (-1 == close(pipe_head_grep_fileno[0]))
    {
        perror(argv[0]);
        return 3;
    }

    if (-1 == close(pipe_head_grep_fileno[1]))
    {
        perror(argv[0]);
        return 3;
    }

    if (-1 == close(pipe_grep_wc_fileno[0]))
    {
        perror(argv[0]);
        return 3;
    }

    if (-1 == close(pipe_grep_wc_fileno[1]))
    {
        perror(argv[0]);
        return 3;
    }

    waitpid(head_pid, NULL, 0);
    waitpid(grep_pid, NULL, 0);

    int status;
    if (-1 == waitpid(wc_pid, &status, 0))
        perror(argv[0]);

    return WEXITSTATUS(status);
}