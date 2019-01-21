#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

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
    int exit_status = 1;
    for (int i = 1; i < argc; i++)
    {
        exit_status = fork_exec(argv[0], argv[i]);
        if (0 == exit_status)
            return exit_status;
    }
    return exit_status;
}