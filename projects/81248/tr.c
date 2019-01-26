#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define REQUIRED_ARGS 1

void handle_flags(const char* args, bool* complement, bool* delete, bool* nub)
{
    for(int i = 1; args[i] != 0; ++i)
    {
        switch (args[i])
        {
            case 'c':
                *complement = true;
                break;
            case 'd':
                *delete = true;
                break;
            case 's':
                *nub = true;
                break;
        }
    }
}

char last(const char* str)
{
    char curr = str[0];

    for(int i = 0; str[i] != 0; ++i)
    {
        curr = str[i];
    }

    return curr;
}

int main (int argc, const char* const * argv)
{
    if (argc < REQUIRED_ARGS + 1)
    {
        return 1;
    }

    bool complement = false;

    bool delete = false;

    bool nub = false;

    int argstart = 1;

    if (argv[1][0] == '-')
    {
        ++argstart;
        handle_flags(argv[1], &complement, &delete, &nub);
    }

    char mapping[256] = {0};

    const char* const inset = argv[argstart];
    const char* const outset = argv[argstart + 1];

    if (complement)
    {
        char subst_char = last(outset);

        // set everything to the required char
        for(int i = 0; i < 256; ++i)
        {
            mapping[i] = subst_char;
        }

        // only unset those that are our in inset
        // we get a complement this way
        for(int i = 0; inset[i] != 0; ++i)
        {
            mapping[(int)inset[i]] = inset[i];
        }
    }
    else
    {
        for(int i = 0; i < 256; ++i)
        {
            mapping[i] = i;
        }

        int outindex = 0;

        for(int i = 0; inset[i] != 0; ++i)
        {
            mapping[(int)inset[i]] = outset[outindex];

            // if we are at the end of the outset we need to not move
            if(!outset[outindex + 1] == 0)
            {
                ++outindex;
            }
        }
    }

    int curr = 0;

    while((curr = getchar()) != EOF)
    {
        putchar(mapping[curr]);
    }

    return 0;
}
