#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void find(char* str, char separator[100])
{
    int i;
    int size = strlen(str);
    char buff[2];
    for(i = 3; i<size -1; i++)
    {
        buff[0]=str[i];
        buff[1]='\0';
        strcat(separator, &buff);
    }
}

void printNum(int start, int diff, int end, char* separator)
{
    while(start<=end)
    {
        printf("%d",start);
        start+=diff;
        if(start<=end)
            printf("%s",separator);
    }
}

void printNumW(int start, int diff, int end, char* separator)
{
    char buff[100];
	sprintf(buff, "%d", end);  // itoa(3) е нестандартна функция!

    int n = strlen(buff);
    while(start<=end)
    {
        printf("%0*d", n, start);
        start+=diff;
        if(start<=end)
            printf("%s",separator);
    }
}

int main(int argc, char* argv[])
{
    int start,end,diff;
    char separator[100];
    strcpy(separator,"");
    if(argc < 2)
    {
        perror(argv[0]);
        return 1;
    }

    if(argc == 2)
    {
        start =1;
        end = atoi(argv[1]);
        diff=1;
        strcpy(separator,"\n");
        printNum(start,diff,end,separator);
    }
    else
    {
        if(argv[1][0]=='-' && argv[1][1]=='s')
        {
            find(argv[1], separator);
            if(argv[2][0]=='-')
            {
                if(argc==4)
                {
                    end = atoi(argv[3]);
                    start = 1;
                    diff = 1;
                }
                else if(argc==5)
                {
                    start = atoi(argv[3]);
                    diff = 1;
                    end = atoi(argv[4]);
                }
                else if(argc==6)
                {
                    start = atoi(argv[3]);
                    diff = atoi(argv[4]);
                    end = atoi(argv[5]);
                }
                printNumW(start,diff,end,separator);
            }
            else
            {
                if(argc==3)
                {
                    end = atoi(argv[2]);
                    start = 1;
                    diff = 1;
                }
                else if(argc==4)
                {
                    start = atoi(argv[2]);
                    diff = 1;
                    end = atoi(argv[3]);
                }
                else if(argc==5)
                {
                    start = atoi(argv[2]);
                    diff = atoi(argv[3]);
                    end = atoi(argv[4]);
                }
                printNum(start,diff,end,separator);
            }
        }
        else if(argv[1][0]=='-' && argv[1][1]=='w')
        {
            if(argv[2][0]=='-')
            {
                find(argv[2], separator);
                if(argc==4)
                {
                    end = atoi(argv[3]);
                    start = 1;
                    diff = 1;
                }
                else if(argc==5)
                {
                    start = atoi(argv[3]);
                    diff = 1;
                    end = atoi(argv[4]);
                }
                else if(argc==6)
                {
                    start = atoi(argv[3]);
                    diff = atoi(argv[4]);
                    end = atoi(argv[5]);
                }
            }
            else
            {
                strcpy(separator,"\n");
                if(argc==3)
                {
                    end = atoi(argv[2]);
                    start = 1;
                    diff = 1;
                }
                else if(argc==4)
                {
                    start = atoi(argv[2]);
                    diff = 1;
                    end = atoi(argv[3]);
                }
                else if(argc==5)
                {
                    start = atoi(argv[2]);
                    diff = atoi(argv[3]);
                    end = atoi(argv[4]);
                }
            }
            printNumW(start,diff,end,separator);
        }
        else
        {
            strcpy(separator,"\n");
            if(argc==3)
            {
                start = atoi(argv[1]);
                end = atoi(argv[2]);
                diff=1;
            }
            else if(argc==4)
            {
                start = atoi(argv[1]);
                diff = atoi(argv[2]);
                end = atoi(argv[3]);
            }
            printNum(start,diff,end,separator);
        }

    }

    return 0;
}
