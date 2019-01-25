#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_STD_FILENO 2

#define REQUIRED_ARG_COUNT 1

static ssize_t total_line_count;

static void tail(int input_fileno)
{
    char buffer;
    ssize_t read_count, current_line_count = 0;
    while (current_line_count < total_line_count - 1 && (read_count = read(input_fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_count)
        {
            perror("read");
            exit(3);
        }

        if ('\n' == buffer)
            current_line_count++;
    }

    while ((read_count = read(input_fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_count)
        {
            perror("read");
            exit(3);
        }

        write(STDOUT_FILENO, &buffer, 1);
    }
}

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    total_line_count = atol(argv[1]);
    if (total_line_count < 0)
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
            tail(MAX_STD_FILENO + i);
    }
    else
        tail(STDIN_FILENO);

    return 0;
}
