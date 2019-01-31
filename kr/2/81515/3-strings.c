#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define REQUIRED_ARGUMENTS 0
#define BUFF_SIZE 1024

void Perform_Task(int fd)
{
    int symbolsRead = 0;
    char readBuff[BUFF_SIZE];
    char validAscii[BUFF_SIZE];
    int ind = 0;
    int count = 0;
    while((symbolsRead = read(fd, readBuff, BUFF_SIZE)) > 0)
    {
        int i;
        for(i = 0; i < symbolsRead; ++i)
        {
            if((int)readBuff[i] < 32 || (int)readBuff[i] > 126)
            {
                if(count >= 4)
                {
                    write(STDOUT_FILENO, validAscii, count % BUFF_SIZE);
                }
                ind = 0;
                count = 0;
            }
            else
            {
                validAscii[ind] = readBuff[i];
                ++ind;
                if(ind == BUFF_SIZE)
                {
                    write(STDOUT_FILENO, validAscii, BUFF_SIZE);
                    ind %= BUFF_SIZE;
                }
                ++count;
            }
        }
    }

    if(count >= 4)
    {
        write(STDOUT_FILENO, validAscii, count % BUFF_SIZE);
    }
}

int main(int argc, char **argv)
{
    if(argc < REQUIRED_ARGUMENTS + 1)
    {
        exit(1);
    }

    if(argc == 1)
    {
        Perform_Task(STDIN_FILENO);
        return 0;
    }

    int i;
    for(i = 1; i < argc; ++i)
    {
        int fd;
        if((fd = open(argv[i], O_RDONLY)) < 0)
        {
            perror("Task3.c: Open: ");
            exit(2);
        }
        Perform_Task(fd);
        close(fd);
    }

    return 0;
}