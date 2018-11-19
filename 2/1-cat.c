#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 1000

int main(int argc, const char* const* argv)
{
    char buffer[BUFFER_SIZE];
    ssize_t count;
    while ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) != 0)
    {
        if (-1 == count)
        {
            perror(argv[0]);
            return 1;
        }

        write(STDOUT_FILENO, buffer, count);
    }

    return 0;
}
