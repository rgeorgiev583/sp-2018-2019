#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_FILE_MODE 0644

#define REQUIRED_ARG_COUNT 3

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    ssize_t position = atol(argv[1]);

    const char* word = argv[2];

    int fileno = open(argv[3], O_WRONLY, DEFAULT_FILE_MODE);
    if (-1 == fileno)
    {
        perror("open");
        exit(5);
    }

    if (-1 == lseek(fileno, position, position >= 0 ? SEEK_SET : SEEK_END))
    {
        perror("lseek");
        exit(6);
    }

    if (-1 == write(fileno, word, strlen(word)))
    {
        perror("write");
        exit(4);
    }

    return 0;
}