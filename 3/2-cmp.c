#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>

#define NEWLINE_CHARACTER '\n'

#define REQUIRED_ARGUMENT_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARGUMENT_COUNT + 1)
        return 1;

    int fileno1 = open(argv[1], O_RDONLY);
    int fileno2 = open(argv[2], O_RDONLY);
    char buffer1, buffer2;
    size_t current_byte = 0;
    size_t current_line = 0;
    bool is_eof1 = false, is_eof2 = false;
    do
    {
        is_eof1 = read(fileno1, &buffer1, 1) <= 0;
        is_eof2 = read(fileno2, &buffer2, 1) <= 0;
        if (is_eof1 || is_eof2)
            break;

        current_byte++;
        if (NEWLINE_CHARACTER == buffer1)
            current_line++;
    }
    while (buffer1 == buffer2);

    if (!is_eof1 || !is_eof2)
    {
        printf("%s %s differ: byte %u, line %u\n", argv[1], argv[2], current_byte, current_line);
    }

    return 0;
}
