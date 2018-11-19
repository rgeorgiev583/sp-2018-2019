#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 1

int main(int argc, char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    pid_t pid = fork();
    if (-1 == pid)
    {
        perror(argv[0]);
        return 2;
    }
    else if (0 == pid && -1 == execvp(argv[1], argv + 1))
    {
        perror(argv[0]);
        return 3;
    }

    int status;
    if (-1 == wait(&status))
    {
        perror(argv[0]);
        return 4;
    }

    printf("\n%d\n", WEXITSTATUS(status));
    return 0;
}