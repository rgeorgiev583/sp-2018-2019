#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

#define REQUIRED_ARG_COUNT 4

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    ssize_t from_position = atol(argv[1]);
    if (from_position < 0)
        exit(2);

    ssize_t to_position = atol(argv[2]);
    if (to_position < 0)
        exit(2);
    
    const char* char_str = argv[3];

    if (strlen(char_str) > 1)
        exit(2);

    int fileno = open(argv[4], O_WRONLY, DEFAULT_FILE_MODE);
    if (fileno < 0)
    {
        perror("open");
        exit(5);
    }

    if (-1 == lseek(fileno, from_position, from_position >= 0 ? SEEK_SET : SEEK_END))
    {
        perror("lseek");
        exit(6);
    }

    for (ssize_t i = from_position; i <= to_position; i++)
    {
        if (-1 == write(fileno, char_str, 1))
        {
            perror("write");
            exit(4);
        }
    }

    return 0;
}