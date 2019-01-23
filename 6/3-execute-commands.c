#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* const* argv)
{
    for (int i = 1; i < argc; i++)
    {
        switch (fork())
        {
        case -1:
            perror("fork");
            exit(9);

        case 0:
            if (-1 == execlp(argv[i], argv[i], NULL))
            {
                perror("exec");
                exit(8);
            }
        }
    }

    for (int i = 1; i < argc; i++)
        wait(NULL);

    return 0;
}