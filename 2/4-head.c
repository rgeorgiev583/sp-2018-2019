#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    ssize_t total_count = atol(argv[1]);
    if (total_count < 0)
        exit(2);

    int input_fileno = open(argv[2], O_RDONLY);
    if (-1 == input_fileno)
    {
        perror("open");
        exit(5);
    }

    char buffer;
    ssize_t read_count, current_count = 0;
    while (current_count < total_count && (read_count = read(input_fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_count)
        {
            perror("read");
            exit(3);
        }

        write(STDOUT_FILENO, &buffer, 1);

        current_count++;
    }

    return 0;
}
