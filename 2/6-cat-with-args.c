#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1000
#define FD_OFFSET 2

int main(int argc, char const* const* argv)
{
    for (int i = 1; i < argc; i++)
        creat(argv[i], 0644);

    char buffer[BUFFER_SIZE];
    size_t count;
    while (count = read(0, buffer, BUFFER_SIZE))
        for (int i = 1; i < argc; i++)
            write(FD_OFFSET + i, buffer, count);

    return 0;
}
