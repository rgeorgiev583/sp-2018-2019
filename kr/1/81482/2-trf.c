#include <unistd.h>
#include <fcntl.h>

int main(int argc, const char* const* argv) {
    int input = open(argv[4], O_RDONLY);
    int output = creat(argv[3], 0644);
    dup2(input, STDIN_FILENO);
    dup2(output, STDOUT_FILENO);
    execlp("tr", "tr", argv[1], argv[2], NULL);

    return 0;
}