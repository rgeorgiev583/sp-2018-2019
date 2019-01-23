#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* const* argv)
{
    if (-1 == execvp("cat", argv))
    {
        perror(argv[0]);
        exit(8);
    }

    return 0;
}