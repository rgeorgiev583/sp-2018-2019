#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define REQUIRED_ARGUMENTS 3
#define BUFF_SIZE 1024

int main(int argc, char **argv)
{
    if(argc < REQUIRED_ARGUMENTS + 1)
    {
        exit(1);
    }

    int n = atoi(argv[1]);
    n += 81515 % 23 + 4;
    int writeFd;
    if((writeFd = open(argv[3], O_WRONLY | O_CREAT, 0666 )) < 0)
    {
        perror("Task1: Open write: ");
        exit(2);
    }

    int readFd;
    if((readFd = open(argv[2], O_RDONLY)) < 0)
    {
        perror("Task1: Open read: ");
        exit(3);
    }

    lseek(writeFd, n, SEEK_END);
    int symbolsRead = 0;
    char buff[BUFF_SIZE];
    while((symbolsRead = read(readFd, buff, BUFF_SIZE)) > 0)
    {
        write(writeFd, buff, symbolsRead);
    }

    return 0;
}