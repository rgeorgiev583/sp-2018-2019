#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, const char** argv) {
    int pipe_fileno[2];
    if (-1 == pipe(pipe_fileno)) {
        perror(argv[0]);
        return 1;
    }
    pid_t who_pid = fork();
    if (who_pid == -1) {
        perror(argv[0]);
        return 2;
    }
    if (who_pid == 0) {
        if (-1 == close(pipe_fileno[0])) {
            perror(argv[0]);
            return 3;
        }
        if (-1 == dup2(pipe_fileno[1], STDOUT_FILENO)) {
            perror(argv[0]);
            return 4;
        }
        argv[0] = "sort";
        if (-1 == execvp("sort", (char* const*)argv)) {
            perror(argv[0]);
            return 5;
        }
    }
    pid_t uniq_pid = fork();
    if (uniq_pid == -1) {
        perror(argv[0]);
        return 2;
    }
    if (uniq_pid == 0) {
        if (-1 == dup2(pipe_fileno[0], STDIN_FILENO)) {
            perror(argv[0]);
            return 4;
        }
        if (-1 == close(pipe_fileno[1])) {
            perror(argv[0]);
            return 3;
        }
        if (-1 == execlp("uniq", "uniq", NULL)) {
            perror(argv[0]);
            return 5;
        }
    }
    if (-1 == close(pipe_fileno[0])) {
        perror(argv[0]);
        return 3;
    }
    if (-1 == close(pipe_fileno[1])) {
        perror(argv[0]);
        return 3;
    }
    waitpid(who_pid, NULL, 0);
    int status;
    if (-1 == waitpid(uniq_pid, &status, 0))
        perror(argv[0]);
    return WEXITSTATUS(status);
}