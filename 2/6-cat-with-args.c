#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1000
#define MAX_STD_FILENO 2

#define REQUIRED_ARG_COUNT 0

int main(int argc, char const* const* argv)
{
    char buffer[BUFFER_SIZE];
    size_t count;
    if (argc > REQUIRED_ARG_COUNT + 1)
    {
        for (int i = REQUIRED_ARG_COUNT + 1; i < argc; i++)
            open(argv[i], O_RDONLY);

        for (int i = 1; i < argc - REQUIRED_ARG_COUNT; i++)
            while ((count = read(MAX_STD_FILENO + i, buffer, BUFFER_SIZE)) > 0)
                write(STDOUT_FILENO, buffer, count);
    }
    else
    {
        while ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
            write(STDOUT_FILENO, buffer, count);
    }


    return 0;
}
