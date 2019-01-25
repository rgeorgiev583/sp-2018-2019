#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

static int fork_exec(const char* command_name)
{
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork");
        exit(9);

    case 0:
        if (-1 == execlp(command_name, command_name, NULL))
        {
            perror("exec");
            exit(8);
        }
    }

    int status;
    wait(&status);
    return WEXITSTATUS(status);
}

int main(int argc, const char* const* argv)
{
    int exit_status = 0;
    for (int i = 1; i < argc; i++)
    {
        exit_status = fork_exec(argv[i]);
        if (0 != exit_status)
            return exit_status;
    }
    return exit_status;
}