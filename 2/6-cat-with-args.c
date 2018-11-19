#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFFER_SIZE 1000
#define MAX_STD_FILENO 2

#define REQUIRED_ARG_COUNT 0

int main(int argc, char const* const* argv)
{
    char buffer[BUFFER_SIZE];
    ssize_t count;
    if (argc > REQUIRED_ARG_COUNT + 1)
    {
        for (int i = REQUIRED_ARG_COUNT + 1; i < argc; i++)
        {
            if (-1 == open(argv[i], O_RDONLY))
            {
                perror(argv[0]);
                return 1;
            }
        }

        for (int i = 1; i < argc - REQUIRED_ARG_COUNT; i++)
        {
            while ((count = read(MAX_STD_FILENO + i, buffer, BUFFER_SIZE)) != 0)
            {
                if (-1 == count)
                {
                    perror(argv[0]);
                    return 2;
                }

                write(STDOUT_FILENO, buffer, count);
            }
        }
    }
    else
    {
        while ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) != 0)
        {
            if (-1 == count)
            {
                perror(argv[0]);
                return 2;
            }

            write(STDOUT_FILENO, buffer, count);
        }
    }

    return 0;
}
