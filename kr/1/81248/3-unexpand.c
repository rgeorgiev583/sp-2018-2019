#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

const char spaces[9] = "        ";

ssize_t unexpandfile(int fd)
{
    char inner_buf[8];
    char tab[2] = "\t";
    char bufsingle = 0;
    ssize_t count = 0;
    ssize_t inner_count = 0;

    while ((count = (read(fd, &bufsingle, 1))) != 0)
    {
        if (count == -1)
        {
            return 1;
        }

        if(bufsingle == ' ')
        {
            // go back one char
            if(lseek(fd, -1, SEEK_CUR) == -1)
            {
                return 2;
            }

            inner_count = read(fd, inner_buf, 8);
            if(inner_count == -1)
            {
                return 3;
            }

            if(strncmp(spaces, inner_buf, 8) == 0)
            {
                write(STDOUT_FILENO, tab, 1);
            }
            else
            {
                write(STDOUT_FILENO, inner_buf, inner_count);
            }

        }
        else
        {
            write(STDOUT_FILENO, &bufsingle, count);
        }
    }
    return 0;
}

int main(int argc, const char* const* argv)
{
    if (argc == 1)
    {
        int status = unexpandfile(STDIN_FILENO);
        if(status != 0)
        {
            perror(argv[0]);
            return status;
        }
    }
    else
    {
        int num_files = argc - 1;
        int curr_fd = 0;

        for (int i = 0; i < num_files; ++i)
        {
            curr_fd = open(argv[i + 1], O_RDONLY);

            if(curr_fd == -1)
            {
                perror(argv[0]);
            }
            else
            {
                if(unexpandfile(curr_fd) != 0)
                {
                    perror(argv[0]);
                }
            }

            close(curr_fd);
        }
    }

    return 0;
}
