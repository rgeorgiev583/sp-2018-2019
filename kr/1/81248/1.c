#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define FN 81248
#define MAX_BUFFER 1000

int main(int argc, const char* const* argv)
{
    if (argc < 4)
    {
        return 1;
    }

    const int n = atoi(argv[1]);
    const int offset = n + FN % 29 + 3;

    int file1_fd = open(argv[2], O_RDONLY);


    if(file1_fd == -1)
    {
        perror(argv[0]);
        return 2;
    }

    int file2_fd = open(argv[3], O_WRONLY | O_CREAT, 0644);
    if(file1_fd == -1)
    {
        close(file1_fd);
        perror(argv[0]);
        return 3;
    }

    if (lseek(file2_fd, offset, SEEK_END) == -1)
    {
        close(file1_fd);
        close(file2_fd);
        perror(argv[0]);
        return 4;
    }

    ssize_t count = 0;
    char buf[MAX_BUFFER];

    while ((count = read(file1_fd, buf, MAX_BUFFER)) != 0)
    {
        if (count == -1)
        {
            close(file1_fd);
            close(file2_fd);
            perror(argv[0]);
            return 5;
        }

        if(write(file2_fd, buf, count) == -1)
        {
            close(file1_fd);
            close(file2_fd);
            perror(argv[0]);
            return 6;
        }
    }

    close(file1_fd);
    close(file2_fd);

    return 0;
}
