#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int fileno = open(argv[1], O_WRONLY | O_APPEND);
    if (-1 == fileno)
    {
        perror(argv[0]);
        return 5;
    }

    lseek(fileno, 0, SEEK_SET);

    if (-1 == write(fileno, argv[2], strlen(argv[2])))
    {
        perror(argv[0]);
        return 4;
    }

    return 0;
}
