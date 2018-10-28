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
        return 1;

    int from_position = atoi(argv[1]);
    int to_position = atoi(argv[2]);
    int char_length = strlen(argv[3]);
    int fileno = open(argv[4], O_WRONLY, DEFAULT_FILE_MODE);
    if (fileno < 0)
    {
        perror("could not open file for writing");
        return 2;
    }

    lseek(fileno, from_position, from_position >= 0 ? SEEK_SET : SEEK_END);
    for (size_t i = from_position; i <= to_position; i++)
        write(fileno, argv[3], char_length);

    return 0;
}