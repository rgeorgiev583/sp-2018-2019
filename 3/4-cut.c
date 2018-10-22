#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000
#define FD_OFFSET 2
#define NEWLINE_CHARACTER '\n'

#define REQUIRED_ARGUMENT_COUNT 2

void cut(int fileno, size_t from_position, size_t to_position)
{
    size_t current_position = 0;
    char buffer;
    while (read(fileno, &buffer, 1) > 0)
    {
        if (NEWLINE_CHARACTER == buffer)
        {
            current_position = 0;
            write(STDOUT_FILENO, &buffer, 1);
        }
        else
        {
            current_position++;
            if (current_position >= from_position && current_position <= to_position)
                write(STDOUT_FILENO, &buffer, 1);
        }
    }
}

int main(int argc, char const* const* argv)
{
    if (argc < REQUIRED_ARGUMENT_COUNT + 1)
        return 1;

    size_t from_position = atoi(argv[1]);
    size_t to_position = atoi(argv[2]);

    for (int i = REQUIRED_ARGUMENT_COUNT + 1; i < argc; i++)
        open(argv[i], O_RDONLY);

    if (argc > REQUIRED_ARGUMENT_COUNT + 1)
    {
        for (int i = 1; i < argc - REQUIRED_ARGUMENT_COUNT; i++)
            cut(FD_OFFSET + i, from_position, to_position);
    }
    else
        cut(STDIN_FILENO, from_position, to_position);

    return 0;
}
