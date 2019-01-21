#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, const char* const* argv)
{
    int pipe_fileno[2];
    if (-1 == pipe(pipe_fileno))
    {
        perror(argv[0]);
        return 12;
    }

    pid_t ls_pid = fork();
    switch (ls_pid)
    {
    case -1:
        perror(argv[0]);
        return 9;

    case 0:
        close(pipe_fileno[0]);
        dup2(pipe_fileno[1], STDOUT_FILENO);

        if (-1 == execlp("ls", "ls", NULL))
        {
            perror(argv[0]);
            return 8;
        }
    }

    pid_t wc_pid = fork();
    switch (wc_pid)
    {
    case -1:
        perror(argv[0]);
        return 9;

    case 0:
        dup2(pipe_fileno[0], STDIN_FILENO);
        close(pipe_fileno[1]);

        if (-1 == execlp("wc", "wc", "-l", NULL))
        {
            perror(argv[0]);
            return 8;
        }
    }

    close(pipe_fileno[0]);
    close(pipe_fileno[1]);

    waitpid(ls_pid, NULL, 0);

    int status;
    waitpid(wc_pid, &status, 0);
    return WEXITSTATUS(status);
}