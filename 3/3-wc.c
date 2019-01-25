#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_STD_FILENO 2

static ssize_t total_char_count = 0, total_line_count = 0;

void wc(int fileno, const char* filename)
{
    char buffer;
    ssize_t read_count, char_count = 0, line_count = 0;
    while ((read_count = read(fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_count)
        {
            perror("read");
            exit(3);
        }

        char_count++;
        if ('\n' == buffer)
            line_count++;
    }

    if (NULL == filename)
        printf(" %ld %ld\n", line_count, char_count);
    else
        printf(" %ld %ld %s\n", line_count, char_count, filename);

    total_char_count += char_count;
    total_line_count += line_count;
}

int main(int argc, const char* const* argv)
{
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
            wc(MAX_STD_FILENO + i, argv[i]);
    }
    else
        wc(STDIN_FILENO, NULL);

    printf(" %ld %ld total\n", total_line_count, total_char_count);

    return 0;
}
