#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

#define REQUIRED_ARG_COUNT 2

int main(int argc, char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    const char* set2 = argv[1];

    const char* set1 = argv[2];

    if (argc > REQUIRED_ARG_COUNT + 1)
    {
        for (int i = 3; i < argc; i++)
        {
            switch (fork())
            {
            case -1:
                perror("fork");
                exit(9);

            case 0:
                {
                    int input_fileno = open(argv[i], O_RDONLY);
                    if (-1 == input_fileno)
                    {
                        perror("open");
                        exit(5);
                    }

                    dup2(input_fileno, STDIN_FILENO);
                    if (-1 == execlp("tr", "tr", set1, set2, NULL))
                    {
                        perror("exec");
                        exit(8);
                    }
                }
            }
        }

        for (int i = 3; i < argc; i++)
            wait(NULL);
    }
    else
    {
        if (-1 == execlp("tr", "tr", set1, set2, NULL))
        {
            perror("exec");
            exit(8);
        }
    }

    return 0;
}