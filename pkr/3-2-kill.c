#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#define REQUIRED_ARG_COUNT 2

int main(int argc, const char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    if (argv[1][0] != '-')
        exit(2);

    int signal_number = atoi(&argv[1][1]);
    if (0 == signal_number)
        exit(2);

    for (int i = 2; i < argc; i++)
    {
        int pid = atoi(argv[i]);
        if (0 == pid)
            exit(2);

        if (-1 == kill(pid, signal_number))
        {
            perror("kill");
            exit(13);
        }
    }

    return 0;
}