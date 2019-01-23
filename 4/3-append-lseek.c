#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    int output_fileno = open(argv[1], O_WRONLY | O_APPEND);
    if (-1 == output_fileno)
    {
        perror(argv[0]);
        exit(5);
    }

    lseek(output_fileno, 0, SEEK_SET);

    if (-1 == write(output_fileno, argv[2], strlen(argv[2])))
    {
        perror(argv[0]);
        exit(4);
    }

    return 0;
}
