#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>


#define REQUIRED_ARG_COUNT 1

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    int input_fileno = open(argv[1], O_RDONLY);
    if (-1 == input_fileno)
    {
        perror(argv[0]);
        exit(5);
    }

    dup2(input_fileno, STDIN_FILENO);

    char buffer[BUFSIZ];
    size_t read_count;
    while ((read_count = read(STDIN_FILENO, buffer, BUFSIZ)) > 0)
    {
        if (-1 == read_count)
        {
            perror(argv[0]);
            exit(3);
        }

        write(STDOUT_FILENO, &buffer, read_count);
    }

    return 0;
}