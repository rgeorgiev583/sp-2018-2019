#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define ARG_COUNT 2

int main (int argc, const char* const * argv)
{
    if (argc < ARG_COUNT + 1)
    {
        return 1;
    }

    char mapping[256] = {0};

    for(int i = 0; i < 256; ++i)
    {
        mapping[i] = i;
    }

    const char* const inset = argv[1];
    const char* const outset = argv[2];
    int outindex = 0;

    for(int i = 0; inset[i] != 0; ++i)
    {
        mapping[(int)inset[i]] = outset[outindex];

        if(!outset[outindex + 1] == 0)
        {
            ++outindex;
        }
    }

    int curr = 0;

    while((curr = getc(stdin)) != EOF)
    {
        putchar(mapping[curr]);
    }

    return 0;
}
