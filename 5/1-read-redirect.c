#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1000

#define REQUIRED_ARG_COUNT 1

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int fileno = open(argv[1], O_RDONLY);
    dup2(fileno, STDIN_FILENO);

    char buffer[BUFFER_SIZE];
    size_t count;
    while ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
        write(STDOUT_FILENO, &buffer, count);

    return 0;
}