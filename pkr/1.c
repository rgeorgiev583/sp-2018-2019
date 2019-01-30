#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 3

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    ssize_t hole_size = atol(argv[1]);

    int output_fileno = open(argv[2], O_WRONLY);
    if (-1 == output_fileno)
    {
        perror("open");
        exit(5);
    }

    int input_fileno = open(argv[3], O_RDONLY);
    if (-1 == input_fileno)
    {
        perror("open");
        exit(5);
    }

    lseek(output_fileno, hole_size, SEEK_END);

    char buffer[BUFSIZ];
    ssize_t read_count;
    while ((read_count = read(input_fileno, &buffer, BUFSIZ)) != 0)
    {
        if (-1 == input_fileno)
        {
            perror("read");
            exit(3);
        }

        if (-1 == write(output_fileno, &buffer, read_count))
        {
            perror("write");
            exit(4);
        }
    }

    return 0;
}