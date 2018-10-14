#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1000

int main(int argc, const char* const* argv)
{
    if (argc < 2)
        return 1;

    int fd = creat(argv[1], 0644);
    char buffer[BUFFER_SIZE];
    size_t count;
    while (count = read(0, buffer, BUFFER_SIZE))
    {
        write(1, buffer, count);
        write(fd, buffer, count);
    }

    return 0;
}
