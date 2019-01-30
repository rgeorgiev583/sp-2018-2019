#include <unistd.h>
#include <fcntl.h>

#define arg_count 4
#define DEFAULT_FILE_MODE 0644

int main(int argc, char* const* argv)
{
    if (argc != (arg_count + 1))
    {
        perror(argv[0]);
        return 1;
    }

    int input_fd = open(argv[4], O_RDONLY);
    int output_fd = creat(argv[3], DEFAULT_FILE_MODE);
    dup2(input_fd, STDIN_FILENO);
    dup2(output_fd, STDOUT_FILENO);
    execlp("tr", "tr", argv[1], argv[2], NULL);
}