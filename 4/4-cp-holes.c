#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define DEFAULT_FILE_MODE 0644
#define ZERO_CHAR '\0'

#define REQUIRED_ARG_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int input_fileno = open(argv[1], O_RDONLY);
    if (-1 == input_fileno)
    {
        perror(argv[0]);
        return 5;
    }

    int output_fileno = creat(argv[2], DEFAULT_FILE_MODE);
    if (-1 == output_fileno)
    {
        perror(argv[0]);
        return 5;
    }

    char buffer;
    size_t hole_size = 0;
    ssize_t read_result;
    while ((read_result = read(input_fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_result)
        {
            perror(argv[0]);
            return 3;
        }

        if (ZERO_CHAR == buffer)
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
                return 4;
            }
        }
    }

    return 0;
}
