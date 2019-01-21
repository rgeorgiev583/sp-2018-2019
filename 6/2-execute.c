#include <unistd.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 1

int main(int argc, char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    if (-1 == execvp(argv[1], argv + 1))
    {
        perror(argv[0]);
        return 8;
    }

    return 0;
}