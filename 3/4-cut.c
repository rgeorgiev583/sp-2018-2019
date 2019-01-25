#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_STD_FILENO 2

#define REQUIRED_ARG_COUNT 2

void cut(int fileno, ssize_t from_position_in_line, ssize_t to_position_in_line)
{
    char buffer;
    ssize_t read_count, current_position_in_line = 0;
    while ((read_count = read(fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_count)
        {
            perror("read");
            exit(3);
        }

        if ('\n' == buffer)
        {
            current_position_in_line = 0;

            write(STDOUT_FILENO, &buffer, 1);
        }
        else
        {
            current_position_in_line++;

            if (current_position_in_line >= from_position_in_line && current_position_in_line <= to_position_in_line)
                write(STDOUT_FILENO, &buffer, 1);
        }
    }
}

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    ssize_t from_position_in_line = atol(argv[1]);
    if (from_position_in_line <= 0)
        exit(2);

    ssize_t to_position_in_line = atol(argv[2]);
    if (to_position_in_line <= 0)
        exit(2);

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
            cut(MAX_STD_FILENO + i, from_position_in_line, to_position_in_line);
    }
    else
        cut(STDIN_FILENO, from_position_in_line, to_position_in_line);

    return 0;
}
