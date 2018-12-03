#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1000
#define MAX_ARG_COUNT 100

int main(int argc, const char* const* argv)
{
    while (1)
    {
        write(1, "$ ", 2);
        char command[BUFFER_SIZE];
        ssize_t command_len = read(0, command, BUFFER_SIZE);
        if (-1 == command_len)
        {
            perror(argv[0]);
            return 1;
        }

        command[command_len - 1] = '\0';

        char* command_argv[MAX_ARG_COUNT];
        command_argv[0] = strtok(command, " ");
        int command_arg_count = 0;
        do
        {
            command_arg_count++;
            command_argv[command_arg_count] = strtok(NULL, " ");
        }
        while (NULL != command_argv[command_arg_count]);

        if (0 == strcmp(command_argv[0], "exit") || 0 == strcmp(command_argv[0], "quit"))
            return 0;

        pid_t pid = fork();
        if (-1 == pid)
        {
            perror(argv[0]);
            return 2;
        }
        else if (0 == pid && -1 == execvp(command_argv[0], command_argv))
        {
            fprintf(stderr, "%s: error: command `%s` does not exist\n", argv[0], command_argv[0]);
            return 3;
        }
        else
        {
            int status;
            if (-1 == wait(&status))
            {
                perror(argv[0]);
                return 4;
            }
            else
            {
                int exit_status = WEXITSTATUS(status);
                if (0 != exit_status)
                    fprintf(stderr, "%s: warning: command `%s` (PID %d) exited with a non-zero status code (%d)\n", argv[0], command_argv[0], pid, exit_status);
            }
        }
    }

    return 0;
}
