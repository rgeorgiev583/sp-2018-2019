#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define TAB_WIDTH 8

static void expand(int input_fileno)
{
    char buffer;
    ssize_t read_result;
    while ((read_result = read(input_fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_result)
        {
            perror("read");
            exit(3);
        }

        if ('\t' == buffer)
        {
            for (int i = 0; i < TAB_WIDTH; i++)
                write(STDOUT_FILENO, " ", 1);
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
                exit(5);
            }

            expand(input_fileno);
        }
    }
    else
        expand(STDIN_FILENO);

    return 0;
}