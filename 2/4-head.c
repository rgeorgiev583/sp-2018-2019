#include <unistd.h>
#include <fcntl.h>

int main(int argc, const char* const* argv)
{
    if (argc < 2)
        return 1;

    int fd = open(argv[1], O_RDONLY);
    int total_count = atoi(argv[2]);
    char buffer;
    size_t current_count = 0;
    while (current_count < total_count && read(fd, &buffer, 1) > 0)
        write(1, &buffer, 1);
    return 0;
}
