#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define NEWLINE_CHARACTER '\n'

#define REQUIRED_ARG_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int fileno1 = open(argv[1], O_RDONLY);
    if (-1 == fileno1)
    {
        perror(argv[0]);
        return 5;
    }

    int fileno2 = open(argv[2], O_RDONLY);
    if (-1 == fileno2)
    {
        perror(argv[0]);
        return 5;
    }

    char buffer1, buffer2;
    size_t current_byte = 0;
    size_t current_line = 0;
    ssize_t read_result1, read_result2;
    do
    {
        read_result1 = read(fileno1, &buffer1, 1);
        if (-1 == read_result1)
        {
            perror(argv[0]);
            return 3;
        }

        read_result2 = read(fileno2, &buffer2, 1);
        if (-1 == read_result2)
        {
            perror(argv[0]);
            return 3;
        }

        if (0 == read_result1 || 0 == read_result2)
            break;

        current_byte++;
        if (NEWLINE_CHARACTER == buffer1)
            current_line++;
    }
    while (buffer1 == buffer2);

    if (0 != read_result1 || 0 != read_result2)
        printf("%s %s differ: byte %lu, line %lu\n", argv[1], argv[2], current_byte, current_line);

    return 0;
}
