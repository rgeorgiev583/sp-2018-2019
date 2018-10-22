#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000
#define FD_OFFSET 2
#define NEWLINE_CHARACTER '\n'

#define REQUIRED_ARGUMENT_COUNT 1

void tail(int fileno, size_t total_count)
{
    size_t current_count = 0;
    char buffer;
    while (current_count < total_count - 1 && read(fileno, &buffer, 1) > 0)
    {
        if (NEWLINE_CHARACTER == buffer)
            current_count++;
    }

    while (read(fileno, &buffer, 1) > 0)
        write(STDOUT_FILENO, &buffer, 1);
}

int main(int argc, char const* const* argv)
{
    if (argc < REQUIRED_ARGUMENT_COUNT + 1)
        return 1;

    size_t total_count = atoi(argv[1]);

    for (int i = REQUIRED_ARGUMENT_COUNT + 1; i < argc; i++)
        open(argv[i], O_RDONLY);

    if (argc > REQUIRED_ARGUMENT_COUNT + 1)
    {
        for (int i = 1; i < argc - REQUIRED_ARGUMENT_COUNT; i++)
            tail(FD_OFFSET + i, total_count);
    }
    else
        tail(STDIN_FILENO, total_count);

    return 0;
}
