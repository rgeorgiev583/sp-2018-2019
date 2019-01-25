#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_STD_FILENO 2

void wc(int fileno, ssize_t* char_count, ssize_t* line_count)
{
    char buffer;
    ssize_t read_count;
    *char_count = 0;
    *line_count = 0;
    while ((read_count = read(fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_count)
        {
            perror("read");
            exit(3);
        }

        (*char_count)++;
        if ('\n' == buffer)
            (*line_count)++;
    }
}

int main(int argc, char const* const* argv)
{
    ssize_t total_char_count = 0, total_line_count = 0;

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (-1 == open(argv[i], O_RDONLY))
            {
                perror("open");
                exit(5);
            }
        }

        for (int i = 1; i < argc; i++)
        {
            ssize_t char_count, line_count;
            wc(MAX_STD_FILENO + i, &char_count, &line_count);
            printf(" %ld %ld %s\n", line_count, char_count, argv[i]);
            total_char_count += char_count;
            total_line_count += line_count;
        }
    }
    else
    {
        ssize_t char_count = 0, line_count = 0;
        wc(STDIN_FILENO, &char_count, &line_count);
        printf(" %ld %ld\n", line_count, char_count);
        total_char_count += char_count;
        total_line_count += line_count;
    }

    printf(" %ld %ld total\n", total_line_count, total_char_count);

    return 0;
}
