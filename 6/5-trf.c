#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 4

#define DEFAULT_FILE_MODE 0644

int main(int argc, char* const* argv)
{
    int input_fileno = open(argv[3], O_RDONLY);
    if (-1 == input_fileno)
    {
        perror("open");
        exit(5);
    }

    int output_fileno = creat(argv[4], DEFAULT_FILE_MODE);
    if (-1 == output_fileno)
    {
        perror("open");
        exit(5);
    }

    dup2(input_fileno, STDIN_FILENO);
    dup2(output_fileno, STDOUT_FILENO);

    if (-1 == execlp("tr", "tr", argv[1], argv[2], NULL))
    {
        perror("exec");
        exit(8);
    }

    return 0;
}