#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>


#define REQUIRED_ARG_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    int fileno1 = open(argv[1], O_RDONLY);
    if (-1 == fileno1)
    {
        perror("open");
        exit(5);
    }

    int fileno2 = open(argv[2], O_RDONLY);
    if (-1 == fileno2)
    {
        perror("open");
        exit(5);
    }

    char buffer1, buffer2;
    ssize_t read_count1, read_count2, current_char_position = 0, current_line_position = 0;
    do
    {
        read_count1 = read(fileno1, &buffer1, 1);
        if (-1 == read_count1)
        {
            perror("read");
            exit(3);
        }

        read_count2 = read(fileno2, &buffer2, 1);
        if (-1 == read_count2)
        {
            perror("read");
            exit(3);
        }

        if (0 == read_count1 || 0 == read_count2)
            break;

        current_char_position++;
        if ('\n' == buffer1)
            current_line_position++;
    }
    while (buffer1 == buffer2);

    if (0 != read_count1 || 0 != read_count2)
        printf("%s %s differ: byte %ld, line %ld\n", argv[1], argv[2], current_char_position, current_line_position);

    return 0;
}
