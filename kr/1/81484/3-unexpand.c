#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define SPACE_CHAR ' '
#define MAX_STD_FILENO 2

int main(int argc, char const* const* argv)
{
    char buffer[4];
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            int fdi = open(argv[i], O_RDONLY);
            if (fdi == -1)
            {
                perror(argv[0]);
                return 1;
            }

            size_t space_size = 0;
            //while (read(fdi, &buffer, 1))
            while (read(fdi, buffer, 1))  // променено
            {
                if (SPACE_CHAR == buffer[0])
                {
                    space_size++;
                    if (space_size == 8)
                    {
                        //buffer = "\t";
                        strcpy(buffer, "\t");  // променено с цел компилация
                        //write(STDOUT_FILENO, &buffer, 2);
                        write(STDOUT_FILENO, buffer, 1);  // променено
                        space_size = 0;
                    }
                }
                //if (SPACE_CHAR != buffer[0])
                else  // променено
                {
                    if (space_size == 0)
                        write(STDOUT_FILENO, buffer, 1);
                    if (space_size != 0)
                    {
                        //char c = " ";
                        char c = ' ';  // променено с цел избягване на предупреждения
                        for (int j = 0; j < space_size; j++)
                            //write(STDOUT_FILENO, c, 1);
                            write(STDOUT_FILENO, &c, 1);  // променено
                        write(STDOUT_FILENO, buffer, 1);
                        space_size = 0;  // добавено
                    }
                }
            }
        }
    }
    else
    {
        size_t space_size = 0;
        size_t count;  // добавено с цел компилация
        while ((count = read(STDIN_FILENO, buffer, 1)) != 0)
        {
            if (-1 == count)
            {
                perror(argv[0]);
                return 2;
            }

            if (SPACE_CHAR == buffer[0])
            {
                space_size++;
                if (space_size == 8)
                {
                    //buffer = "\t";
                    strcpy(buffer, "\t");  // променено с цел компилация
                    //write(STDOUT_FILENO, &buffer, 2);
                    write(STDOUT_FILENO, buffer, 1);  // променено
                    space_size = 0;
                }
            }
            //if (SPACE_CHAR != buffer[0])
            else  // променено
            {
                if (space_size == 0)
                    write(STDOUT_FILENO, buffer, 1);
                if (space_size != 0)
                {
                    //char c = " ";
                    char c = ' ';  // променено с цел избягване на предупреждения
                    for (int j = 0; j < space_size; j++)
                        //write(STDOUT_FILENO, c, 1);
                        write(STDOUT_FILENO, &c, 1);  // променено
                    write(STDOUT_FILENO, buffer, 1);
                    space_size = 0;  // добавено
                }
            }
        }
    }

    return 0;
}