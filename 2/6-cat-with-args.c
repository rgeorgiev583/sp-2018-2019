#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_STD_FILENO 2

void cat(int input_fileno)
{
    char buffer[BUFSIZ];
    ssize_t read_count;
    while ((read_count = read(input_fileno, buffer, BUFSIZ)) != 0)
    {
        if (-1 == read_count)
        {
            perror("read");
            exit(3);
        }

        write(STDOUT_FILENO, buffer, read_count);
    }
}

int main(int argc, char const* const* argv)
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
            cat(MAX_STD_FILENO + i);
    }
    else
        cat(STDIN_FILENO);

    return 0;
}
