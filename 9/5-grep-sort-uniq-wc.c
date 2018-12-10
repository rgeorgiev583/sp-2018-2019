#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, const char** argv)
{
    int pipe_grep_sort_fileno[2];
    if (-1 == pipe(pipe_grep_sort_fileno))
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
        if (-1 == close(pipe_grep_sort_fileno[0]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == dup2(pipe_grep_sort_fileno[1], STDOUT_FILENO))
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

    int pipe_sort_uniq_fileno[2];
    if (-1 == pipe(pipe_sort_uniq_fileno))
    {
        perror(argv[0]);
        return 1;
    }

    pid_t sort_pid = fork();
    switch (sort_pid)
    {
    case -1:
        perror(argv[0]);
        return 2;

    case 0:
        if (-1 == dup2(pipe_grep_sort_fileno[0], STDIN_FILENO))
        {
            perror(argv[0]);
            return 4;
        }

        if (-1 == close(pipe_grep_sort_fileno[1]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == close(pipe_sort_uniq_fileno[0]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == dup2(pipe_sort_uniq_fileno[1], STDOUT_FILENO))
        {
            perror(argv[0]);
            return 4;
        }

        if (-1 == execlp("sort", "sort", NULL))
        {
            perror(argv[0]);
            return 5;
        }
    }

    int pipe_uniq_wc_fileno[2];
    if (-1 == pipe(pipe_uniq_wc_fileno))
    {
        perror(argv[0]);
        return 1;
    }

    pid_t uniq_pid = fork();
    switch (uniq_pid)
    {
    case -1:
        perror(argv[0]);
        return 2;

    case 0:
        if (-1 == close(pipe_grep_sort_fileno[0]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == close(pipe_grep_sort_fileno[1]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == dup2(pipe_sort_uniq_fileno[0], STDIN_FILENO))
        {
            perror(argv[0]);
            return 4;
        }

        if (-1 == close(pipe_sort_uniq_fileno[1]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == close(pipe_uniq_wc_fileno[0]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == dup2(pipe_uniq_wc_fileno[1], STDOUT_FILENO))
        {
            perror(argv[0]);
            return 4;
        }

        if (-1 == execlp("uniq", "uniq", NULL))
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
        if (-1 == close(pipe_grep_sort_fileno[0]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == close(pipe_grep_sort_fileno[1]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == close(pipe_sort_uniq_fileno[0]))
        {
            perror(argv[0]);
            return 4;
        }

        if (-1 == close(pipe_sort_uniq_fileno[1]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == dup2(pipe_uniq_wc_fileno[0], STDIN_FILENO))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == close(pipe_uniq_wc_fileno[1]))
        {
            perror(argv[0]);
            return 4;
        }

        if (-1 == execlp("wc", "wc", "-l", NULL))
        {
            perror(argv[0]);
            return 5;
        }
    }

    if (-1 == close(pipe_grep_sort_fileno[0]))
    {
        perror(argv[0]);
        return 3;
    }

    if (-1 == close(pipe_grep_sort_fileno[1]))
    {
        perror(argv[0]);
        return 3;
    }

    if (-1 == close(pipe_sort_uniq_fileno[0]))
    {
        perror(argv[0]);
        return 3;
    }

    if (-1 == close(pipe_sort_uniq_fileno[1]))
    {
        perror(argv[0]);
        return 3;
    }

    if (-1 == close(pipe_uniq_wc_fileno[0]))
    {
        perror(argv[0]);
        return 3;
    }

    if (-1 == close(pipe_uniq_wc_fileno[1]))
    {
        perror(argv[0]);
        return 3;
    }

    waitpid(grep_pid, NULL, 0);
    waitpid(sort_pid, NULL, 0);
    waitpid(uniq_pid, NULL, 0);

    int status;
    if (-1 == waitpid(wc_pid, &status, 0))
        perror(argv[0]);

    return WEXITSTATUS(status);
}