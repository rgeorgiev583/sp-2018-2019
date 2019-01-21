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

void sh(const char* program_name, FILE* input_file)
{
    while (1)
    {
        if (stdin == input_file)
            printf("$ ");

        char* command_buffer = NULL;
        size_t command_buffer_size = 0;
        ssize_t command_len = getline(&command_buffer, &command_buffer_size, input_file);
        if (-1 == command_len)
            break;

        command_buffer[command_len - 1] = '\0';

        char* command_argv[MAX_ARG_COUNT];
        command_argv[0] = strtok(command_buffer, " ");
        int arg_count = 0;
        do
        {
            arg_count++;
            command_argv[arg_count] = strtok(NULL, " ");
        }
        while (NULL != command_argv[arg_count]);

        if (0 == strcmp(command_argv[0], "exit") || 0 == strcmp(command_argv[0], "quit"))
            exit(0);

        int op_count = 0, op_types[MAX_ARG_COUNT], subcommand_argv_positions[MAX_ARG_COUNT];
        op_types[op_count] = 0;
        subcommand_argv_positions[0] = 0;
        for (int i = 0; i < arg_count; i++)
        {
            if (0 == strcmp(command_argv[i], "&&"))
            {
                op_count++;
                op_types[op_count] = 1;
                subcommand_argv_positions[op_count] = i + 1;
                command_argv[i] = NULL;
            }
            else if (0 == strcmp(command_argv[i], "||"))
            {
                op_count++;
                op_types[op_count] = 2;
                subcommand_argv_positions[op_count] = i + 1;
                command_argv[i] = NULL;
            }
        }
        op_count++;

        int exit_status = 0;
        for (int i = 0; i < op_count; i++)
        {
            if ((1 == op_types[i] && 0 != exit_status) || (2 == op_types[i] && 0 == exit_status))
                break;

            int subcommand_argv_position = subcommand_argv_positions[i];
            exit_status = fork_exec(program_name, (char* const*)command_argv + subcommand_argv_position);
        }

        free(command_buffer);
    }

    if (stdin != input_file)
        fclose(input_file);
}

int main(int argc, const char* const* argv)
{
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            FILE* input_file = fopen(argv[i], "r");
            if (NULL == input_file)
            {
                perror(argv[0]);
                return 2;
            }

            sh(argv[0], input_file);
        }
    }
    else
        sh(argv[0], stdin);

    return 0;
}
