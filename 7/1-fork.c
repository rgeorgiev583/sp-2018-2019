#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main()
{
    pid_t pid = fork();
    if (!pid)
        return 0;

    int status;
    wait(&status);
    printf("%d\n", pid);
    printf("%d\n", WEXITSTATUS(status));
    return 0;
}