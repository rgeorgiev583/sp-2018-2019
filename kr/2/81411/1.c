#include <unistd.h>
#include <fcntl.h>

#define REQUIRED_ARGS 3

int main(int argc, char** argv) {
    if (argc < REQUIRED_ARGS + 1) {
        exit(1);
    }

    int n = atoi(argv[1]);
    n += 81411 % 23 + 4;

    int writefd = open(argv[3], O_WRONLY | O_CREAT, 0666);
    if (writefd < 0) {
        perror(argv[0]);
        exit(2);
    }

    int readfd = open(argv[2], O_RDONLY);
    if (readfd < 0) {
        perror(argv[0]);
        exit(3);
    }

    lseek(writefd, n, SEEK_END);
    char buffer[1024];
    int symbol = 0;
    while ((symbol = read(readfd, buffer, 1024)) > 0) {
        write(writefd, buffer, symbol);
    }

    return 0;
}