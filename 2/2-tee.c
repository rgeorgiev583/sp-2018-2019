#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1000
#define DEFAULT_FILE_MODE 0644

int main(int argc, const char* const* argv)
{
    if (argc < 2)
        return 1;

    int output_fileno = creat(argv[1], DEFAULT_FILE_MODE);
    char buffer[BUFFER_SIZE];
    size_t count;
    while ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
    {
        write(STDOUT_FILENO, buffer, count);
        write(output_fileno, buffer, count);
    }

    return 0;
}
