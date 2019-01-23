#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_FILE_MODE 0644

#define REQUIRED_ARG_COUNT 2

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

    int output_fileno = creat(argv[2], DEFAULT_FILE_MODE);
    if (-1 == output_fileno)
    {
        perror(argv[0]);
        exit(5);
    }

    char buffer;
    size_t hole_size = 0;
    ssize_t read_count;
    while ((read_count = read(input_fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_count)
        {
            perror(argv[0]);
            exit(3);
        }

        if ('\0' == buffer)
            hole_size++;
        else
        {
            if (hole_size > 0)
            {
                lseek(output_fileno, hole_size, SEEK_CUR);
                hole_size = 0;
            }

            if (-1 == write(output_fileno, &buffer, 1))
            {
                perror(argv[0]);
                exit(4);
            }
        }
    }

    return 0;
}
