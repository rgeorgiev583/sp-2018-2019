#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, const char* const* argv)
{
    pid_t pid = fork();
    if (-1 == pid)
    {
        perror(argv[0]);
        return 1;
    }
    else if (0 == pid)
        return 0;

    int status;
    if (-1 == wait(&status))
    {
        perror(argv[0]);
        return 2;
    }

    printf("%d\n", pid);
    printf("%d\n", WEXITSTATUS(status));
    return 0;
}