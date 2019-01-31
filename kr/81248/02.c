#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_BUFFER 1000

int main (int argc, const char* const * argv)
{
    if (argc < 5)
    {
        return 1;
    }

    char mapping[256] = {0};
    for(int i = 0; i < 256; ++i)
    {
        mapping[i] = i;
    }

    int infile_fd = open(argv[4], O_RDONLY);
    if(infile_fd == -1)
    {
        perror(argv[0]);
        return 2;
    }

    int outfile_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(outfile_fd == -1)
    {
        close(infile_fd);
        perror(argv[0]);
        return 3;
    }

    const char* const inset = argv[1];
    const char* const outset = argv[2];
    int outindex = 0; // maybe this could be in the for, don't remember

    for(int i = 0; inset[i] != 0; ++i)
    {
        mapping[(int)inset[i]] = outset[outindex];
        ++outindex;
        if(outset[outindex] == 0)
        {
            outindex = 0;
        }
    }

    ssize_t count = 0;
    char buf[MAX_BUFFER];

    while((count = read(infile_fd, buf, MAX_BUFFER)) != 0)
    {
        if (count == -1)
        {
            close(infile_fd);
            close(outfile_fd);
            perror(argv[0]);
            return 4;
        }

        for(int i = 0; i < MAX_BUFFER; ++i)
        {
            buf[i] = mapping[(int)buf[i]];
        }

        if(write(outfile_fd, buf, count) == -1)
        {
            close(infile_fd);
            close(outfile_fd);
            perror(argv[0]);
            return 5;
        }
    }

    close(infile_fd);
    close(outfile_fd);
    return 0;
}
