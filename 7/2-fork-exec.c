#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 1

int main(int argc, char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork");
        exit(9);

    case 0:
        if (-1 == execvp(argv[1], argv + 1))
        {
            perror("exec");
            exit(8);
        }
    }

    int status;
    wait(&status);
    printf("\n%d\n", WEXITSTATUS(status));
    return 0;
}