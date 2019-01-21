#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 1

int main(int argc, char* const* argv)
{
    int input_fileno = open(argv[1], O_RDONLY);
    if (-1 == input_fileno)
    {
        perror(argv[0]);
        return 5;
    }

    dup2(input_fileno, STDIN_FILENO);

    if (-1 == execlp("wc", "wc", NULL))
    {
        perror(argv[0]);
        return 8;
    }

    return 0;
}