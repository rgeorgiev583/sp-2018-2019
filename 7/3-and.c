#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define REQUIRED_ARG_COUNT 2

int fork_exec(const char* command_name)
{
    pid_t pid = fork();
    if (!pid)
        execlp(command_name, command_name, NULL);

    int status;
    wait(&status);
    return WEXITSTATUS(status);
}

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int exit_status = fork_exec(argv[1]);
    if (0 != exit_status)
        return exit_status;

    exit_status = fork_exec(argv[2]);
    return exit_status;
}