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
        printf("%s: error: command `%s` does not exist\n", program_name, command_argv[0]);
        exit(3);
    }

    int status;
    if (-1 == wait(&status))
    {
        perror(program_name);
        exit(4);
    }

    return WEXITSTATUS(status);
}

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
        int arg_count = 0;
        do
        {
            arg_count++;
            command_argv[arg_count] = strtok(NULL, " ");
        }
        while (NULL != command_argv[arg_count]);

        if (0 == strcmp(command_argv[0], "exit") || 0 == strcmp(command_argv[0], "quit"))
            return 0;

        int op_type = 0, next_subcommand_argv_position = -1;
        for (int i = 0; i < arg_count; i++)
        {
            if (0 == strcmp(command_argv[i], "&&"))
            {
                op_type = 1;
                next_subcommand_argv_position = i + 1;
                command_argv[i] = NULL;
            }
            else if (0 == strcmp(command_argv[i], "||"))
            {
                op_type = 2;
                next_subcommand_argv_position = i + 1;
                command_argv[i] = NULL;
            }
        }

        int exit_status = fork_exec(command_argv[0], command_argv);
        if ((1 == op_type && 0 != exit_status) || (2 == op_type && 0 == exit_status))
            continue;

        fork_exec(command_argv[next_subcommand_argv_position], (char* const*)command_argv + next_subcommand_argv_position);
    }

    return 0;
}
