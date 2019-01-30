#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define TAB_WIDTH 8

static void unexpand(int input_fileno)
{
    ssize_t blank_length = 0;

    char buffer;
    ssize_t read_count;
    while ((read_count = read(input_fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_count)
        {
            perror("read");
            exit(5);
        }

        if (' ' == buffer)
        {
            if (blank_length == TAB_WIDTH)
            {
                write(STDOUT_FILENO, "\t", 1);
                blank_length = 0;
            }
            else
                blank_length++;
        }
        else
            write(STDOUT_FILENO, &buffer, 1);
    }
}

int main(int argc, const char* const* argv)
{
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            int input_fileno = open(argv[i], O_RDONLY);
            if (-1 == input_fileno)
            {
                perror("open");
                exit(4);
            }

            unexpand(input_fileno);
        }
    }
    else
        unexpand(STDIN_FILENO);

    return 0;
}