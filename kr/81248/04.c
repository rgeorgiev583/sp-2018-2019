#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>


int main(int argc, char* const* argv)
{
    int pipe_arr[2] = { 0 };

    if(pipe(pipe_arr) == -1)
    {
        perror(argv[0]);
        return 1;
    }

    pid_t sort_pid = fork();

    if(sort_pid == -1)
    {
        close(pipe_arr[0]);
        close(pipe_arr[1]);
        perror(argv[0]);
        return 2;
    }

    if(sort_pid == 0)
    {
        close(pipe_arr[0]);
        dup2(pipe_arr[1], STDOUT_FILENO);
        execvp("sort", argv);
    }

    pid_t uniq_pid = fork();

    if(uniq_pid == -1)
    {
        close(pipe_arr[0]);
        close(pipe_arr[1]);
        perror(argv[0]);
        return 2;
    }

    if(uniq_pid == 0)
    {
        close(pipe_arr[1]);
        dup2(pipe_arr[0], STDIN_FILENO);
        execlp("uniq", "uniq", NULL);
    }

    close(pipe_arr[0]);
    close(pipe_arr[1]);

    int status = 0;

    waitpid(sort_pid, &status, 0);
    if(WEXITSTATUS(status) != 0)
    {
        kill(uniq_pid, 9);
        return WEXITSTATUS(status);
    }

    waitpid(uniq_pid, &status, 0);

    return WEXITSTATUS(status);
}
