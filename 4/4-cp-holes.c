#include <unistd.h>
#include <fcntl.h>

#define DEFAULT_FILE_MODE 0644
#define ZERO_CHAR '\0'

#define REQUIRED_ARG_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int input_fileno = open(argv[1], O_RDONLY);
    int output_fileno = creat(argv[2], DEFAULT_FILE_MODE);
    char buffer;
    size_t hole_size = 0;
    while (read(input_fileno, &buffer, 1))
    {
        if (ZERO_CHAR == buffer)
            hole_size++;
        else
        {
            if (hole_size > 0)
            {
                lseek(output_fileno, hole_size, SEEK_CUR);
                hole_size = 0;
            }

            write(output_fileno, &buffer, 1);
        }
    }

    return 0;
}
