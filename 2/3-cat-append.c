#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1000

int main(int argc, const char* const* argv)
{
    if (argc < 2)
        return 1;

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
    char buffer[BUFFER_SIZE];
    size_t count;
    while (count = read(0, buffer, BUFFER_SIZE))
        write(fd, buffer, count);

    return 0;
}
