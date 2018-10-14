#include <unistd.h>
#include <fcntl.h>

int main(int argc, const char* const* argv)
{
    if (argc < 2)
        return 1;

    int fd = open(argv[1], O_RDONLY, 0644);
    int total_count = atoi(argv[2]);
    char c;
    size_t current_count = 0;
    while (current_count < total_count && (count = read(fd, buffer, 1)))
        write(1, buffer, 1);
    return 0;
}
