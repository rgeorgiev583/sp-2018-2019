#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define REQUIRED_ARG_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int fileno = open(argv[1], O_WRONLY | O_APPEND);
    lseek(fileno, 0, SEEK_SET);
    write(fileno, argv[2], strlen(argv[2]));

    return 0;
}
