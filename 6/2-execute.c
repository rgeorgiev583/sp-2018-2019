#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 1

int main(int argc, char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    if (-1 == execvp(argv[1], argv + 1))
    {
        perror("exec");
        exit(8);
    }

    return 0;
}