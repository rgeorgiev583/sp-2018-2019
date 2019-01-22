#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_STD_FILENO 2

#define REQUIRED_ARG_COUNT 1

void tail(const char* program_name, int fileno, size_t total_line_count)
{
    size_t current_line_count = 0;
    char buffer;
    ssize_t read_count;
    while (current_line_count < total_line_count - 1 && (read_count = read(fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_count)
        {
            perror(program_name);
            exit(3);
        }

        if ('\n' == buffer)
            current_line_count++;
    }

    while (read(fileno, &buffer, 1) > 0)
        write(STDOUT_FILENO, &buffer, 1);
}

int main(int argc, char const* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    size_t total_line_count = atoi(argv[1]);
    if (total_line_count < 0)
    {
        perror(argv[0]);
        return 2;
    }

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
            tail(argv[0], MAX_STD_FILENO + i, total_line_count);
    }
    else
        tail(argv[0], STDIN_FILENO, total_line_count);

    return 0;
}
