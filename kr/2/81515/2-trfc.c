#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define REQUIRED_ARGUMENTS 2

int Fork_Exec(char *fileName, char **argv)
{
    int pid = fork();
    if(pid != 0)
    {
        return pid;
    }

    int readFd = open(fileName, O_RDONLY);
    if(readFd < 0)
    {
        perror("Task2: Fork_Exec: ");
        exit(3);
    }

    dup2(readFd, STDIN_FILENO);
    close(readFd);
    if((execlp("tr", "tr", argv[1], argv[2], NULL)) < 0)
    {
        perror("Task2: Exec: ");
        exit(4);
    }

    return -1;
}

int main(int argc, char **argv)
{
    if(argc < REQUIRED_ARGUMENTS + 1)
    {
        exit(1);
    }

    if(argc == 3 && (execlp("tr", "tr", argv[1], argv[2], NULL)) < 0)
    {
        perror("Task2: Exec: ");
        exit(2);
    }

    int i = 3;
    int pid;
    for(; i < argc; ++i)
    {
        pid = Fork_Exec(argv[i], argv);
    }

    waitpid(pid, NULL, 0);
    return 0;
}