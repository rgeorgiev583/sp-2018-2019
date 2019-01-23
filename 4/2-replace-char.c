#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_FILE_MODE 0644

#define REQUIRED_ARG_COUNT 4

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    if (strlen(argv[3]) > 1)
        exit(2);

    int from_position = atoi(argv[1]);
    if (from_position < 0)
    {
        perror(argv[0]);
        exit(2);
    }

    int to_position = atoi(argv[2]);
    if (to_position < 0)
    {
        perror(argv[0]);
        exit(2);
    }

    int fileno = open(argv[4], O_WRONLY, DEFAULT_FILE_MODE);
    if (fileno < 0)
    {
        perror(argv[0]);
        exit(5);
    }

    if (-1 == lseek(fileno, from_position, from_position >= 0 ? SEEK_SET : SEEK_END))
    {
        perror(argv[0]);
        exit(6);
    }

    for (size_t i = from_position; i <= to_position; i++)
    {
        if (-1 == write(fileno, argv[3], 1))
        {
            perror(argv[0]);
            exit(4);
        }
    }

    return 0;
}