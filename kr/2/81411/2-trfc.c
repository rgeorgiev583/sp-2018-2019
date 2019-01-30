#include <unistd.h>
#include <fcntl.h>

#define REQUIRED_ARGS 3

int fork_exec(char* filename, char** argv) {
    int pid = fork();
    if (pid != 0)
        return pid;

    int readfd = open(filename, O_RDONLY);
    if (readfd < 0)
        exit(3);

    dup2(readfd, STDIN_FILENO);
    close(readfd);
    if (execlp("tr", "tr", argv[1], argv[2], NULL) < 0)
        exit(4);

    return -1;
}

int main(int argc, char** argv) {
    if (argc < REQUIRED_ARGS + 1) {
        exit(1);
    }

    if (argc == 3 && (execlp("tr", "tr", argv[1], argv[2], NULL) < 0)) {
        perror(argv[0]);
        exit(2);
    }

    int i;
    int pid;
    for (i = 3; i < argc; i++) {
        pid = fork_exec(argv[i], argv);
    }

    waitpid(pid, NULL, 0);

    return 0;
}