#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_STD_FILENO 2

#define REQUIRED_ARG_COUNT 0

void wc(const char* program_name, int fileno, size_t* char_count, size_t* line_count)
{
    char buffer;
    *char_count = 0;
    *line_count = 0;
    ssize_t read_count;
    while ((read_count = read(fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_count)
        {
            perror(program_name);
            exit(3);
        }

        (*char_count)++;
        if ('\n' == buffer)
            (*line_count)++;
    }
}

int main(int argc, char const* const* argv)
{
    size_t total_char_count = 0, total_line_count = 0;

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
            size_t char_count, line_count;
            wc(argv[0], MAX_STD_FILENO + i, &char_count, &line_count);
            printf(" %u %u %s\n", line_count, char_count, argv[REQUIRED_ARG_COUNT + i]);
            total_char_count += char_count;
            total_line_count += line_count;
        }
    }
    else
    {
        size_t char_count = 0, line_count = 0;
        wc(argv[0], STDIN_FILENO, &char_count, &line_count);
        printf(" %u %u\n", line_count, char_count);
        total_char_count += char_count;
        total_line_count += line_count;
    }

    printf(" %u %u total\n", total_line_count, total_char_count);

    return 0;
}
