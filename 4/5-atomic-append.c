#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_FILE_MODE 0644

#define REQUIRED_ARG_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int base_arg = 1;
    bool do_lseek = false;

    if (!strcmp(argv[1], "-l"))
    {
        base_arg = 2;
        do_lseek = true;
    }
    
    int flags = O_WRONLY | O_CREAT;
    if (do_lseek)
        flags |= O_APPEND;
    int fileno = open(argv[base_arg], flags, DEFAULT_FILE_MODE);
    int byte_count = atol(argv[base_arg + 1]);
    
    for (int i = 0; i < byte_count; i++)
    {
        if (do_lseek)
            lseek(fileno, 0, SEEK_END);
        write(fileno, "x", 1);
    }

    return 0;
}
