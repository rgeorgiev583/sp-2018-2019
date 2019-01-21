#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_FILE_MODE 0644
#define APPEND_FILE_FLAGS O_WRONLY | O_CREAT | O_APPEND
#define BUFFER_SIZE 1000
#define MAX_ARG_COUNT 100

int fork_exec(const char* program_name, char* const* command_argv, const char* output_filename, const char* append_filename, const char* input_filename)
{
    pid_t pid = fork();
    if (-1 == pid)
    {
        perror(program_name);
        exit(9);
    }
    else if (0 == pid)
    {
        if (NULL != output_filename)
        {
            int output_fileno = creat(output_filename, DEFAULT_FILE_MODE);
            if (-1 == output_fileno)
            {
                perror(program_name);
                exit(5);
            }

            dup2(output_fileno, STDOUT_FILENO);
        }

        if (NULL != append_filename)
        {
            int append_fileno = open(output_filename, APPEND_FILE_FLAGS, DEFAULT_FILE_MODE);
            if (-1 == append_fileno)
            {
                perror(program_name);
                exit(5);
            }

            dup2(append_fileno, STDOUT_FILENO);
        }

        if (NULL != input_filename)
        {
            int input_fileno = open(input_filename, O_RDONLY);
            if (-1 == input_fileno)
            {
                perror(program_name);
                exit(5);
            }

            dup2(input_fileno, STDIN_FILENO);
        }

        if (-1 == execvp(command_argv[0], command_argv))
        {
            fprintf(stderr, "%s: error: command `%s` does not exist\n", program_name, command_argv[0]);
            exit(8);
        }
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
        char command[BUFFER_SIZE];
        ssize_t command_len = read(0, command, BUFFER_SIZE);
        if (-1 == command_len)
        {
            perror(argv[0]);
            return 3;
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
        const char* output_filenames[MAX_ARG_COUNT] = {};
        const char* append_filenames[MAX_ARG_COUNT] = {};
        const char* input_filenames[MAX_ARG_COUNT] = {};
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
                op_types[op_count] = 2;
                next_subcommand_argv_positions[op_count] = i + 1;
                command_argv[i] = NULL;
                op_count++;
            }
            else if (0 == strcmp(command_argv[i], ">"))
            {
                output_filenames[op_count] = command_argv[i + 1];
                command_argv[i] = NULL;
            }
            else if (0 == strcmp(command_argv[i], ">>"))
            {
                append_filenames[op_count] = command_argv[i + 1];
                command_argv[i] = NULL;
            }
            else if (0 == strcmp(command_argv[i], "<"))
            {
                input_filenames[op_count] = command_argv[i + 1];
                command_argv[i] = NULL;
            }
        }

        int exit_status = fork_exec(argv[0], command_argv, output_filenames[0], append_filenames[0], input_filenames[0]);
        for (int i = 0; i < op_count; i++)
        {
            if ((1 == op_types[i] && 0 != exit_status) || (2 == op_types[i] && 0 == exit_status))
            break;

            int subcommand_argv_position = next_subcommand_argv_positions[i];
            exit_status = fork_exec(argv[0], (char* const*)command_argv + subcommand_argv_position, output_filenames[i + 1], append_filenames[i + 1], input_filenames[i + 1]);
        }
    }

    return 0;
}
