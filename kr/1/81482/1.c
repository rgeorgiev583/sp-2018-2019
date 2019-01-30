#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define ARGCOUNT 3

int main(int argc, const char* const* argv) {
    if (argc > ARGCOUNT + 1)
        return 1;
    
    int file1 = open(argv[2], O_RDONLY);
    int file2 = open(argv[3], O_RDWR);
    if (file1 < 0 || file2 < 0) {
        perror(argv[0]);
        return 2;
    }

    int n = atoi(argv[1]);
    lseek(file2, n + 81482 % 29 + 3, SEEK_END);

    char buffer;
    ssize_t count;
    while ((count = read(file1, &buffer, 1)) != 0) {
        write(file2, &buffer, 1);
    }
}