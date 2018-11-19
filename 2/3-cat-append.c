#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1000
#define DEFAULT_FILE_MODE 0644
#define APPEND_FILE_FLAGS O_WRONLY | O_CREAT | O_APPEND

#define REQUIRED_ARG_COUNT 1

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int output_fileno = open(argv[1], APPEND_FILE_FLAGS, DEFAULT_FILE_MODE);
    char buffer[BUFFER_SIZE];
    ssize_t count;
    while ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
        write(output_fileno, buffer, count);

    return 0;
}
