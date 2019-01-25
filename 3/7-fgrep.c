#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_STD_FILENO 2

#define REQUIRED_ARG_COUNT 1
#define MAX_LINE_LENGTH 80

void fgrep(int fileno, const char* pattern)
{
    ssize_t pattern_length = 0;
    while ('\0' != pattern[pattern_length])
        pattern_length++;

    bool is_not_eof = true;
    while (is_not_eof)
    {
        char buffer[MAX_LINE_LENGTH];
        ssize_t read_count, current_line_length = 0;
        while (current_line_length < MAX_LINE_LENGTH && (is_not_eof = read(fileno, &buffer[current_line_length], 1) > 0) && '\n' != buffer[current_line_length])
        {
            if (-1 == read_count)
            {
                perror("read");
                exit(3);
            }

            current_line_length++;
        }

        for (ssize_t i = 0; i < current_line_length - pattern_length + 1; i++)
        {
            ssize_t j;
            for (j = 0; j < pattern_length; j++)
                if (pattern[j] != buffer[i + j])
                    break;

            if (pattern_length == j)
            {
                write(STDOUT_FILENO, buffer, current_line_length);
                write(STDOUT_FILENO, "\n", 1);

                break;
            }
        }
    }
}

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    if (argc > REQUIRED_ARG_COUNT + 1)
    {
        for (int i = REQUIRED_ARG_COUNT + 1; i < argc; i++)
        {
            if (-1 == open(argv[i], O_RDONLY))
            {
                perror("open");
                exit(5);
            }
        }

        for (int i = 1; i < argc - REQUIRED_ARG_COUNT; i++)
            fgrep(MAX_STD_FILENO + i, argv[1]);
    }
    else
        fgrep(STDIN_FILENO, argv[1]);

    return 0;
}
