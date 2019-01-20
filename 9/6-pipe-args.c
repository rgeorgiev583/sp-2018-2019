#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#define MAX_ARG_COUNT 100

int main(int argc, const char** argv)
{
    int pipe_fileno[MAX_ARG_COUNT][2];
    pid_t pid[MAX_ARG_COUNT];

    for (int i = 1; i < argc; i++)
    {
        if (i < argc - 1 && -1 == pipe(pipe_fileno[i - 1]))
        {
            perror(argv[0]);
            return 1;
        }

        pid[i - 1] = fork();
        switch (pid[i - 1])
        {
        case -1:
            perror(argv[0]);
            return 2;

        case 0:
            for (int j = 1; j < i - 1; j++)
            {
                if (-1 == close(pipe_fileno[j - 1][0]))
                {
                    perror(argv[0]);
                    return 3;
                }

                if (-1 == close(pipe_fileno[j - 1][1]))
                {
                    perror(argv[0]);
                    return 3;
                }
            }

            if (i > 1)
            {
                if (-1 == dup2(pipe_fileno[i - 2][0], STDIN_FILENO))
                {
                    perror(argv[0]);
                    return 4;
                }

                if (-1 == close(pipe_fileno[i - 2][1]))
                {
                    perror(argv[0]);
                    return 3;
                }
            }

            if (i < argc - 1)
            {
                if (-1 == close(pipe_fileno[i - 1][0]))
                {
                    perror(argv[0]);
                    return 3;
                }

                if (-1 == dup2(pipe_fileno[i - 1][1], STDOUT_FILENO))
                {
                    perror(argv[0]);
                    return 4;
                }
            }

            if (-1 == execlp(argv[i], argv[i], NULL))
            {
                perror(argv[0]);
                return 5;
            }
        }
    }

    for (int i = 1; i < argc - 1; i++)
    {
        if (-1 == close(pipe_fileno[i - 1][0]))
        {
            perror(argv[0]);
            return 3;
        }

        if (-1 == close(pipe_fileno[i - 1][1]))
        {
            perror(argv[0]);
            return 3;
        }
    }

    for (int i = 1; i < argc - 1; i++)
        waitpid(pid[i - 1], NULL, 0);

    if (argc > 1)
    {
        int status;
        if (-1 == waitpid(pid[argc - 2], &status, 0))
            perror(argv[0]);

        return WEXITSTATUS(status);
    }
    else
        return 0;
}