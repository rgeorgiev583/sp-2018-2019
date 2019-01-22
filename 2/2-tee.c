#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define DEFAULT_FILE_MODE 0644

#define REQUIRED_ARG_COUNT 1

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int output_fileno = creat(argv[1], DEFAULT_FILE_MODE);
    if (-1 == output_fileno)
    {
        perror(argv[0]);
        return 5;
    }

    char buffer[BUFSIZ];
    ssize_t read_count;
    while ((read_count = read(STDIN_FILENO, buffer, BUFSIZ)) != 0)
    {
        if (-1 == read_count)
        {
            perror(argv[0]);
            return 3;
        }

        write(STDOUT_FILENO, buffer, read_count);

        if (-1 == write(output_fileno, buffer, read_count))
        {
            perror(argv[0]);
            return 4;
        }
    }

    return 0;
}
