#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
    int exit_status = 0;
    for (int i = 1; i < argc; i++)
    {
        exit_status = fork_exec(argv[i]);
        if (0 != exit_status)
            return exit_status;
    }
    return exit_status;
}