#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFFER_SIZE 1000

#define REQUIRED_ARG_COUNT 1

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int fileno = open(argv[1], O_RDONLY);
    if (-1 == fileno)
    {
        perror(argv[0]);
        return 5;
    }

    dup2(fileno, STDIN_FILENO);

    char buffer[BUFFER_SIZE];
    size_t count;
    while ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
    {
        if (-1 == count)
        {
            perror(argv[0]);
            return 3;
        }

        write(STDOUT_FILENO, &buffer, count);
    }

    return 0;
}