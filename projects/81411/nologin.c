#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMBER_OF_ARGS 0
#define NOT_OPEN_FILE -1

int main(int argc, const char* const* argv) {
    if (argc>NUMBER_OF_ARGS+1) {
        return 2;
    }

    int fd = open("etc/nologin.txt", O_RDONLY);
    if (fd != NOT_OPEN_FILE) {
        int count;
        char buffer;

        while ((count=read(fd, &buffer, 1))!=0) {
            write(STDOUT_FILENO, &buffer, 1);
        }

    } else {
        write(STDOUT_FILENO, "The account is currently unavailable.", 37);
    }

    return 1;
}
