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

int fork_exec_pipe(const char* program_name, char* const* input_command_argv, char* const* output_command_argv)
{
    int pipe_fileno[2];
    if (-1 == pipe(pipe_fileno))
    {
        perror(program_name);
        exit(12);
    }

    pid_t input_command_pid = fork();
    if (-1 == input_command_pid)
    {
        perror(program_name);
        exit(9);
    }
    else if (0 == input_command_pid)
    {
        close(pipe_fileno[0]);
        dup2(pipe_fileno[1], STDOUT_FILENO);

        if (-1 == execvp(input_command_argv[0], input_command_argv))
        {
            fprintf(stderr, "%s: error: command `%s` does not exist\n", program_name, input_command_argv[0]);
            exit(8);
        }
    }

    pid_t output_command_pid = fork();
    if (-1 == output_command_pid)
    {
        perror(program_name);
        exit(9);
    }
    else if (0 == output_command_pid)
    {
        dup2(pipe_fileno[0], STDIN_FILENO);
        close(pipe_fileno[1]);

        if (-1 == execvp(output_command_argv[0], output_command_argv))
        {
            fprintf(stderr, "%s: error: command `%s` does not exist\n", program_name, output_command_argv[0]);
            exit(8);
        }
    }

    close(pipe_fileno[0]);
    close(pipe_fileno[1]);

    waitpid(input_command_pid, NULL, 0);

    int status;
    waitpid(output_command_pid, &status, 0);
    int exit_status = WEXITSTATUS(status);
    if (0 != exit_status)
        fprintf(stderr, "%s: warning: command `%s` (PID %d) exited with a non-zero status code (%d)\n", program_name, output_command_argv[0], output_command_pid, exit_status);

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

        int operator_count = 0, operator_types[MAX_ARG_COUNT], subcommand_argv_positions[MAX_ARG_COUNT][2];
        operator_types[operator_count] = 0;
        subcommand_argv_positions[0][0] = 0;
        subcommand_argv_positions[0][1] = -1;
        for (int i = 0; i < command_argc; i++)
        {
            if (0 == strcmp(command_argv[i], "&&"))
            {
                operator_count++;
                operator_types[operator_count] = 1;
                subcommand_argv_positions[operator_count][0] = i + 1;
                subcommand_argv_positions[operator_count][1] = -1;
                command_argv[i] = NULL;
            }
            else if (0 == strcmp(command_argv[i], "||"))
            {
                operator_count++;
                operator_types[operator_count] = 2;
                subcommand_argv_positions[operator_count][0] = i + 1;
                subcommand_argv_positions[operator_count][1] = -1;
                command_argv[i] = NULL;
            }
            else if (0 == strcmp(command_argv[i], "|"))
            {
                subcommand_argv_positions[operator_count][1] = i + 1;
                command_argv[i] = NULL;
            }
        }
        operator_count++;

        int exit_status = 0;
        for (int i = 0; i < operator_count; i++)
        {
            if ((1 == operator_types[i] && 0 != exit_status) || (2 == operator_types[i] && 0 == exit_status))
                break;

            int input_subcommand_argv_position = subcommand_argv_positions[i][0];
            int output_subcommand_argv_position = subcommand_argv_positions[i][1];
            if (-1 == output_subcommand_argv_position)
                exit_status = fork_exec(argv[0], (char* const*)command_argv + input_subcommand_argv_position);
            else
                exit_status = fork_exec_pipe(argv[0], (char* const*)command_argv + input_subcommand_argv_position, (char* const*)command_argv + output_subcommand_argv_position);
        }
    }

    return 0;
}
