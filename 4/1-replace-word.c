#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_FILE_MODE 0644

#define REQUIRED_ARG_COUNT 3

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    int position = atoi(argv[1]);

    int word_length = strlen(argv[2]);
    if (word_length < 0)
    {
        perror(argv[0]);
        exit(2);
    }

    int fileno = open(argv[3], O_WRONLY, DEFAULT_FILE_MODE);
    if (-1 == fileno)
    {
        perror(argv[0]);
        exit(5);
    }

    if (-1 == lseek(fileno, position, position >= 0 ? SEEK_SET : SEEK_END))
    {
        perror(argv[0]);
        exit(6);
    }

    if (-1 == write(fileno, argv[2], word_length))
    {
        perror(argv[0]);
        exit(4);
    }

    return 0;
}