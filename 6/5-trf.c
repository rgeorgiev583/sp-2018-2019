#include <unistd.h>
#include <fcntl.h>

#define REQUIRED_ARG_COUNT 4
#define DEFAULT_FILE_MODE 0644

int main(int argc, char* const* argv)
{
    int input_fileno = open(argv[3], O_RDONLY);
    int output_fileno = creat(argv[4], DEFAULT_FILE_MODE);
    dup2(input_fileno, STDIN_FILENO);
    dup2(output_fileno, STDOUT_FILENO);
    execlp("tr", "tr", argv[1], argv[2], NULL);
}