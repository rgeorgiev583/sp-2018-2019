#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 1000

int main(int argc, const char* const* argv)
{
    char buffer[BUFFER_SIZE];
    ssize_t read_count;
    while ((read_count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) != 0)
    {
        if (-1 == read_count)
        {
            perror(argv[0]);
            return 3;
        }

        write(STDOUT_FILENO, buffer, read_count);
    }

    return 0;
}
