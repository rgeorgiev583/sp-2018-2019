#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define REQUIRED_ARGUMENTS 1

int main(int argc, char **argv)
{
    if(argc < REQUIRED_ARGUMENTS + 1)
    {
        exit(1);
    }

    int fd[2];
    pipe(fd);

    int pidofPid = fork();
    if(pidofPid == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        argv[0] = "pidof";
        if((execvp("pidof", argv)) < 0)
        {
            perror("Task4.c: Exec pidof: ");
            exit(2);
        }
    }

    int xargsPid = fork();
    if(xargsPid == 0)
    {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);

        if((execlp("xargs", "xargs", "kill", NULL)) < 0)
        {
            perror("Task4.c: Exec xargs: ");
            exit(2);
        }
    }

    close(fd[0]);
    close(fd[1]);
    waitpid(xargsPid, NULL, 0);

    return 0;
}