#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ARG_COUNT 100

int fork_exec(const char* program_name, char* const* command_argv)
{
    pid_t pid = fork();
    if (-1 == pid)
    {
        perror(program_name);
        exit(9);
    }
    else if (0 == pid && -1 == execvp(command_argv[0], command_argv))
    {
        fprintf(stderr, "%s: error: command `%s` does not exist\n", program_name, command_argv[0]);
        exit(8);
    }

    int status;
    wait(&status);
    int exit_status = WEXITSTATUS(status);
    if (0 != exit_status)
        fprintf(stderr, "%s: warning: command `%s` (PID %d) exited with a non-zero status code (%d)\n", program_name, command_argv[0], pid, exit_status);

    return WEXITSTATUS(status);
}

int main(int argc, const char* const* argv)
{
    while (1)
    {
        write(1, "$ ", 2);
        char command_buffer[BUFSIZ];
        ssize_t command_length = read(0, command_buffer, BUFSIZ);
        if (-1 == command_length)
        {
            perror(argv[0]);
            return 3;
        }

        command_buffer[command_length - 1] = '\0';

        char* command_argv[MAX_ARG_COUNT];
        command_argv[0] = strtok(command_buffer, " ");
        int command_argc = 0;
        do
        {
            command_argc++;
            command_argv[command_argc] = strtok(NULL, " ");
        }
        while (NULL != command_argv[command_argc]);

        if (0 == strcmp(command_argv[0], "exit") || 0 == strcmp(command_argv[0], "quit"))
            return 0;

        int operator_type = 0, next_subcommand_argv_position = -1;
        for (int i = 0; i < command_argc; i++)
        {
            if (0 == strcmp(command_argv[i], "&&"))
            {
                operator_type = 1;
                next_subcommand_argv_position = i + 1;
                command_argv[i] = NULL;
            }
            else if (0 == strcmp(command_argv[i], "||"))
            {
                operator_type = 2;
                next_subcommand_argv_position = i + 1;
                command_argv[i] = NULL;
            }
        }

        int exit_status = fork_exec(command_argv[0], command_argv);
        if ((1 == operator_type && 0 != exit_status) || (2 == operator_type && 0 == exit_status))
            continue;

        fork_exec(argv[0], (char* const*)command_argv + next_subcommand_argv_position);
    }

    return 0;
}
