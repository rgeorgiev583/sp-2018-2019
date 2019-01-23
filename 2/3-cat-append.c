#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_FILE_MODE 0644
#define APPEND_FILE_FLAGS O_WRONLY | O_CREAT | O_APPEND

#define REQUIRED_ARG_COUNT 1

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    int output_fileno = open(argv[1], APPEND_FILE_FLAGS, DEFAULT_FILE_MODE);
    if (-1 == output_fileno)
    {
        perror(argv[0]);
        exit(5);
    }

    char buffer[BUFSIZ];
    ssize_t read_count;
    while ((read_count = read(STDIN_FILENO, buffer, BUFSIZ)) != 0)
    {
        if (-1 == read_count)
        {
            perror(argv[0]);
            exit(3);
        }

        if (-1 == write(output_fileno, buffer, read_count))
        {
            perror(argv[0]);
            exit(4);
        }
    }

    return 0;
}
