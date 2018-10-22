#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFFER_SIZE 1000
#define FD_OFFSET 2
#define NEWLINE_CHARACTER '\n'

#define REQUIRED_ARGUMENT_COUNT 0

void wc(int fileno, size_t* count_bytes, size_t* count_lines)
{
    char buffer;
    *count_bytes = 0;
    *count_lines = 0;
    while (read(fileno, &buffer, 1) > 0)
    {
        (*count_bytes)++;
        if (NEWLINE_CHARACTER == buffer)
            (*count_lines)++;
    }
}

int main(int argc, char const* const* argv)
{
    for (int i = 1; i < argc; i++)
        open(argv[i], O_RDONLY);

    size_t total_count_bytes = 0, total_count_lines = 0;

    if (argc > REQUIRED_ARGUMENT_COUNT + 1)
    {
        for (int i = 1; i < argc - REQUIRED_ARGUMENT_COUNT; i++)
        {
            size_t count_bytes, count_lines;
            wc(FD_OFFSET + i, &count_bytes, &count_lines);
            printf(" %u %u %s\n", count_lines, count_bytes, argv[REQUIRED_ARGUMENT_COUNT + i]);
            total_count_bytes += count_bytes;
            total_count_lines += count_lines;
        }
    }
    else
    {
        size_t count_bytes = 0, count_lines = 0;
        wc(STDIN_FILENO, &count_bytes, &count_lines);
        printf(" %u %u\n", count_lines, count_bytes);
        total_count_bytes += count_bytes;
        total_count_lines += count_lines;
    }

    printf(" %u %u total\n", total_count_lines, total_count_bytes);

    return 0;
}
