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
        return 1;

    int position = atoi(argv[1]);
    int word_length = strlen(argv[2]);
    int fileno = open(argv[3], O_WRONLY, DEFAULT_FILE_MODE);
    if (fileno < 0)
    {
        perror("could not open file for writing");
        return 2;
    }

    lseek(fileno, position, position >= 0 ? SEEK_SET : SEEK_END);
    write(fileno, argv[2], word_length);

    return 0;
}