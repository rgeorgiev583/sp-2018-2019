#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char** argv)
{
    int pipe_grep_sort_fileno[2];
    if (-1 == pipe(pipe_grep_sort_fileno))
    {
        perror(argv[0]);
        exit(12);
    }

    pid_t grep_pid = fork();
    switch (grep_pid)
    {
    case -1:
        perror(argv[0]);
        exit(9);

    case 0:
        close(pipe_grep_sort_fileno[0]);
        dup2(pipe_grep_sort_fileno[1], STDOUT_FILENO);

        argv[0] = "grep";
        if (-1 == execvp("grep", (char* const*)argv))
        {
            perror(argv[0]);
            exit(8);
        }
    }

    int pipe_sort_uniq_fileno[2];
    if (-1 == pipe(pipe_sort_uniq_fileno))
    {
        perror(argv[0]);
        exit(12);
    }

    pid_t sort_pid = fork();
    switch (sort_pid)
    {
    case -1:
        perror(argv[0]);
        exit(9);

    case 0:
        dup2(pipe_grep_sort_fileno[0], STDIN_FILENO);
        close(pipe_grep_sort_fileno[1]);
        close(pipe_sort_uniq_fileno[0]);
        dup2(pipe_sort_uniq_fileno[1], STDOUT_FILENO);

        if (-1 == execlp("sort", "sort", NULL))
        {
            perror(argv[0]);
            exit(8);
        }
    }

    int pipe_uniq_wc_fileno[2];
    if (-1 == pipe(pipe_uniq_wc_fileno))
    {
        perror(argv[0]);
        exit(12);
    }

    pid_t uniq_pid = fork();
    switch (uniq_pid)
    {
    case -1:
        perror(argv[0]);
        exit(9);

    case 0:
        close(pipe_grep_sort_fileno[0]);
        close(pipe_grep_sort_fileno[1]);
        dup2(pipe_sort_uniq_fileno[0], STDIN_FILENO);
        close(pipe_sort_uniq_fileno[1]);
        close(pipe_uniq_wc_fileno[0]);
        dup2(pipe_uniq_wc_fileno[1], STDOUT_FILENO);

        if (-1 == execlp("uniq", "uniq", NULL))
        {
            perror(argv[0]);
            exit(8);
        }
    }

    pid_t wc_pid = fork();
    switch (wc_pid)
    {
    case -1:
        perror(argv[0]);
        exit(9);

    case 0:
        close(pipe_grep_sort_fileno[0]);
        close(pipe_grep_sort_fileno[1]);
        close(pipe_sort_uniq_fileno[0]);
        close(pipe_sort_uniq_fileno[1]);
        dup2(pipe_uniq_wc_fileno[0], STDIN_FILENO);
        close(pipe_uniq_wc_fileno[1]);

        if (-1 == execlp("wc", "wc", "-l", NULL))
        {
            perror(argv[0]);
            exit(8);
        }
    }

    close(pipe_grep_sort_fileno[0]);
    close(pipe_grep_sort_fileno[1]);
    close(pipe_sort_uniq_fileno[0]);
    close(pipe_sort_uniq_fileno[1]);
    close(pipe_uniq_wc_fileno[0]);
    close(pipe_uniq_wc_fileno[1]);

    waitpid(grep_pid, NULL, 0);
    waitpid(sort_pid, NULL, 0);
    waitpid(uniq_pid, NULL, 0);

    int status;
    waitpid(wc_pid, &status, 0);
    return WEXITSTATUS(status);
}