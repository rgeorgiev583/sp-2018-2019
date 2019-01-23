#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char* const* argv)
{
    char buffer[BUFSIZ];
    ssize_t read_count;
    while ((read_count = read(STDIN_FILENO, buffer, BUFSIZ)) != 0)
    {
        if (-1 == read_count)
        {
            perror(argv[0]);
            exit(3);
        }

        write(STDOUT_FILENO, buffer, read_count);
    }

    return 0;
}
