#include <unistd.h>
#include <fcntl.h>

void func(int fd) {
    int symbol = 0;
    char buffer[1000];
    char asci[1000];
    int index = 0;
    int count = 0;

    while ((symbol = read(fd, buffer, 1000)) > 0) {
        int n = 0;
        for (n; n < symbol; n++) {
            if (buffer[n] < 32 || buffer[n] > 126) {
                if (index >= 4) {
                    write(STDOUT_FILENO, asci, index);
                    index = 0;
                }
                count = 0;
            } else {
                index++;
                if (index == 1000) {
                    write(STDOUT_FILENO, asci, 1000);
                    index %= 1024;
                }
                count++;
            }
        }
    }
}

int main(int argc, const char* const* argv) {
    if (argc == 1) {
        func(STDIN_FILENO);
        return 0;
    }

    int i = 1;
    for (i; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            exit(1);
        }
        func(fd);
        close(fd);
    }

    return 0;
}