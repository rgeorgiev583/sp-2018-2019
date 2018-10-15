#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1000
#define DEFAULT_FILE_MODE 0644
#define FD_OFFSET 2

int main(int argc, char const* const* argv)
{
    for (int i = 1; i < argc; i++)
        creat(argv[i], DEFAULT_FILE_MODE);

    char buffer[BUFFER_SIZE];
    size_t count;
    while ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
        for (int i = 1; i < argc; i++)
            write(FD_OFFSET + i, buffer, count);

    return 0;
}
