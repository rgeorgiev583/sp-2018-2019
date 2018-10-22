#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1000
#define DEFAULT_FILE_MODE 0644

#define REQUIRED_ARGUMENT_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARGUMENT_COUNT + 1)
        return 1;

    int input_fileno = open(argv[1], O_RDONLY);
    int output_fileno = creat(argv[2], DEFAULT_FILE_MODE);
    char buffer[BUFFER_SIZE];
    size_t count;
    while ((count = read(input_fileno, buffer, BUFFER_SIZE)) > 0)
        write(output_fileno, buffer, count);

    return 0;
}
