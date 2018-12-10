#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000
#define MAX_ARG_COUNT 100

int fork_exec(const char* program_name, char* const* command_argv)
{
    pid_t pid = fork();
    if (-1 == pid)
    {
        perror(program_name);
        exit(2);
    }
    else if (0 == pid && -1 == execvp(command_argv[0], command_argv))
    {
        fprintf(stderr, "%s: error: command `%s` does not exist\n", program_name, command_argv[0]);
        exit(3);
    }

    int status;
    if (-1 == wait(&status))
    {
        perror(program_name);
        exit(4);
    }
    else
    {
        int exit_status = WEXITSTATUS(status);
        if (0 != exit_status)
            fprintf(stderr, "%s: warning: command `%s` (PID %d) exited with a non-zero status code (%d)\n", program_name, command_argv[0], pid, exit_status);
    }

    return WEXITSTATUS(status);
}

int main(int argc, const char* const* argv)
{
    while (1)
    {
next:
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
        int arg_count = 0;
        do
        {
            arg_count++;
            command_argv[arg_count] = strtok(NULL, " ");
        }
        while (NULL != command_argv[arg_count]);

        if (0 == strcmp(command_argv[0], "exit") || 0 == strcmp(command_argv[0], "quit"))
            return 0;

        int op_count = 0, op_types[MAX_ARG_COUNT], next_subcommand_argv_positions[MAX_ARG_COUNT];
        for (int i = 0; i < arg_count; i++)
        {
            if (0 == strcmp(command_argv[i], "&&"))
            {
                op_types[op_count] = 1;
                next_subcommand_argv_positions[op_count] = i + 1;
                command_argv[i] = NULL;
                op_count++;
            }
            else if (0 == strcmp(command_argv[i], "||"))
            {
                op_types[op_count] = 1;
                next_subcommand_argv_positions[op_count] = i + 1;
                command_argv[i] = NULL;
                op_count++;
            }
        }

        int exit_status = fork_exec(command_argv[0], command_argv);
        for (int i = 0; i < op_count; i++)
        {
            if ((1 == op_types[i] && 0 != exit_status) || (2 == op_types[i] && 0 == exit_status))
                goto next;

            int subcommand_argv_position = next_subcommand_argv_positions[i];
            exit_status = fork_exec(command_argv[subcommand_argv_position], (char* const*)command_argv + subcommand_argv_position);
        }
    }

    return 0;
}
