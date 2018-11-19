#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1000
#define DEFAULT_FILE_MODE 0644

#define REQUIRED_ARG_COUNT 1

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int output_fileno = creat(argv[1], DEFAULT_FILE_MODE);
    char buffer[BUFFER_SIZE];
    ssize_t count;
    while ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
    {
        write(STDOUT_FILENO, buffer, count);
        write(output_fileno, buffer, count);
    }

    return 0;
}
