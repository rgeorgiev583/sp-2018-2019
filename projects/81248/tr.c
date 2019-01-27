#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

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

void do_subst(bool complement, const char* inset, const char* outset)
{
    char mapping[256] = {0};

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
}

void do_delete(bool complement, const char* inset)
{
    // a set containing the elements which should be deleted
    bool should_delete[256] = { 0 };

    if (complement)
    {
        memset(should_delete, 1, 256);
    }

    for(int i = 0; inset[i] != 0; ++i)
    {
        should_delete[(int)inset[i]] ^= 1;
    }

    int curr = 0;

    while((curr = getchar()) != EOF)
    {
        if (!should_delete[curr])
        {
            putchar(curr);
        }
    }
}

int main(int argc, const char* const * argv)
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

    // if we are not deleting we need two input sets
    if(!delete && argc < argstart + 2)
    {
        return 2;
    }

    const char* const inset = argv[argstart];
    const char* const outset = argv[argstart + 1];

    if (delete)
    {
        do_delete(complement, inset);
    }
    else
    {
        do_subst(complement, inset, outset);
    }

    return 0;
}
