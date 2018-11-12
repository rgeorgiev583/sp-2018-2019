#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 1

int main(int argc, char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    pid_t pid = fork();
    if (!pid)
        execvp(argv[1], argv + 1);

    int status;
    wait(&status);
    printf("\n%d\n", WEXITSTATUS(status));
    return 0;
}