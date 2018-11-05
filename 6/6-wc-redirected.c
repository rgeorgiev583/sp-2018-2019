#include <unistd.h>
#include <fcntl.h>

#define REQUIRED_ARG_COUNT 1

int main(int argc, char* const* argv)
{
    int input_fileno = open(argv[1], O_RDONLY);
    dup2(input_fileno, STDIN_FILENO);
    execlp("wc", "wc", NULL);
}