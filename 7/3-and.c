#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 2

int fork_exec(const char* program_name, const char* command_name)
{
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror(program_name);
        exit(9);

    case 0:
        if (-1 == execlp(command_name, command_name, NULL))
        {
            perror(program_name);
            exit(8);
        }
    }

    int status;
    wait(&status);
    return WEXITSTATUS(status);
}

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    int exit_status = fork_exec(argv[0], argv[1]);
    if (0 != exit_status)
        return exit_status;

    exit_status = fork_exec(argv[0], argv[2]);
    return exit_status;
}