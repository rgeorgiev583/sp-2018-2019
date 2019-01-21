#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000
#define MAX_STD_FILENO 2
#define NEWLINE_CHARACTER '\n'

#define REQUIRED_ARG_COUNT 0

void wc(const char* program_name, int fileno, size_t* count_bytes, size_t* count_lines)
{
    char buffer;
    *count_bytes = 0;
    *count_lines = 0;
    ssize_t read_result;
    while ((read_result = read(fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_result)
        {
            perror(program_name);
            exit(3);
        }

        (*count_bytes)++;
        if (NEWLINE_CHARACTER == buffer)
            (*count_lines)++;
    }
}

int main(int argc, char const* const* argv)
{
    size_t total_count_bytes = 0, total_count_lines = 0;

    if (argc > REQUIRED_ARG_COUNT + 1)
    {
        for (int i = REQUIRED_ARG_COUNT + 1; i < argc; i++)
        {
            if (-1 == open(argv[i], O_RDONLY))
            {
                perror(argv[0]);
                return 5;
            }
        }

        for (int i = 1; i < argc - REQUIRED_ARG_COUNT; i++)
        {
            size_t count_bytes, count_lines;
            wc(argv[0], MAX_STD_FILENO + i, &count_bytes, &count_lines);
            printf(" %u %u %s\n", count_lines, count_bytes, argv[REQUIRED_ARG_COUNT + i]);
            total_count_bytes += count_bytes;
            total_count_lines += count_lines;
        }
    }
    else
    {
        size_t count_bytes = 0, count_lines = 0;
        wc(argv[0], STDIN_FILENO, &count_bytes, &count_lines);
        printf(" %u %u\n", count_lines, count_bytes);
        total_count_bytes += count_bytes;
        total_count_lines += count_lines;
    }

    printf(" %u %u total\n", total_count_lines, total_count_bytes);

    return 0;
}
