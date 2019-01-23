#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_FILE_MODE 0644

#define REQUIRED_ARG_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

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
    int output_fileno = open(argv[base_arg], flags, DEFAULT_FILE_MODE);
    if (-1 == output_fileno)
    {
        perror(argv[0]);
        exit(5);
    }

    long byte_count = atol(argv[base_arg + 1]);
    if (byte_count < 0)
    {
        perror(argv[0]);
        exit(2);
    }

    for (int i = 0; i < byte_count; i++)
    {
        if (do_lseek)
            lseek(output_fileno, 0, SEEK_END);

        if (-1 == write(output_fileno, "x", 1))
        {
            perror(argv[0]);
            exit(4);
        }
    }

    return 0;
}
