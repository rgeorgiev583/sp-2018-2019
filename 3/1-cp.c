#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

#define REQUIRED_ARG_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    int input_fileno = open(argv[1], O_RDONLY);
    if (-1 == input_fileno)
    {
        perror("open");
        exit(5);
    }

    int output_fileno = creat(argv[2], DEFAULT_FILE_MODE);
    if (-1 == output_fileno)
    {
        perror("open");
        exit(5);
    }

    char buffer[BUFSIZ];
    ssize_t read_count;
    while ((read_count = read(input_fileno, buffer, BUFSIZ)) != 0)
    {
        if (-1 == read_count)
        {
            perror("read");
            exit(3);
        }

        if (-1 == write(output_fileno, buffer, read_count))
        {
            perror("write");
            exit(4);
        }
    }

    return 0;
}
