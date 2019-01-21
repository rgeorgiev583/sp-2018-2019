#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 1000
#define MAX_STD_FILENO 2
#define NEWLINE_CHAR '\n'
#define STRING_TERMINATOR '\0'

#define REQUIRED_ARG_COUNT 1
#define MAX_LINE_LENGTH 80

void fgrep(const char* program_name, int fileno, const char* pattern)
{
    size_t pattern_length = 0;
    while (STRING_TERMINATOR != pattern[pattern_length])
        pattern_length++;

    bool is_not_eof = true;
    while (is_not_eof)
    {
        char buffer[MAX_LINE_LENGTH];
        size_t current_line_length = 0;
        ssize_t read_count;
        while (current_line_length < MAX_LINE_LENGTH && (is_not_eof = read(fileno, &buffer[current_line_length], 1) > 0) && NEWLINE_CHAR != buffer[current_line_length])
        {
            if (-1 == read_count)
            {
                perror(program_name);
                exit(3);
            }

            current_line_length++;
        }

        for (size_t i = 0; i < current_line_length - pattern_length + 1; i++)
        {
            size_t j;
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

int main(int argc, char const* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    if (argc > REQUIRED_ARG_COUNT + 1)
    {
        for (int i = REQUIRED_ARG_COUNT + 1; i < argc; i++)
            if (-1 == open(argv[i], O_RDONLY))
            {
                perror(argv[0]);
                return 5;
            }

        for (int i = 1; i < argc - REQUIRED_ARG_COUNT; i++)
            fgrep(argv[0], MAX_STD_FILENO + i, argv[1]);
    }
    else
        fgrep(argv[0], STDIN_FILENO, argv[1]);

    return 0;
}
