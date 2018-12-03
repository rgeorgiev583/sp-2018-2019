#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* const* argv)
{
    for (int i = 1; i < argc; i++)
    {
        switch (fork())
        {
        case -1:
            perror(argv[0]);
            return 1;

        case 0:
            if (-1 == execlp(argv[i], argv[i], NULL))
            {
                perror(argv[0]);
                return 2;
            }
        }
    }

    for (int i = 1; i < argc; i++)
        wait(NULL);

    return 0;
}