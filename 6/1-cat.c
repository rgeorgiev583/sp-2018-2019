#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* const* argv)
{
    if (-1 == execvp("cat", argv))
    {
        perror("exec");
        exit(8);
    }

    return 0;
}