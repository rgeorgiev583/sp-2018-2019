#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_FILE_MODE 0644
#define APPEND_FILE_FLAGS O_WRONLY | O_CREAT | O_APPEND
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
        char command_buffer[BUFSIZ];
        ssize_t command_length = read(0, command_buffer, BUFSIZ);
        if (-1 == command_length)
        {
            perror(argv[0]);
            exit(3);
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
            exit(0);

        int operator_count = 0, operator_types[MAX_ARG_COUNT], next_subcommand_argv_positions[MAX_ARG_COUNT];
        const char* output_filenames[MAX_ARG_COUNT] = {};
        const char* append_filenames[MAX_ARG_COUNT] = {};
        const char* input_filenames[MAX_ARG_COUNT] = {};
        for (int i = 0; i < command_argc; i++)
        {
            if (0 == strcmp(command_argv[i], "&&"))
            {
                operator_types[operator_count] = 1;
                next_subcommand_argv_positions[operator_count] = i + 1;
                command_argv[i] = NULL;
                operator_count++;
            }
            else if (0 == strcmp(command_argv[i], "||"))
            {
                operator_types[operator_count] = 2;
                next_subcommand_argv_positions[operator_count] = i + 1;
                command_argv[i] = NULL;
                operator_count++;
            }
            else if (0 == strcmp(command_argv[i], ">"))
            {
                output_filenames[operator_count] = command_argv[i + 1];
                command_argv[i] = NULL;
            }
            else if (0 == strcmp(command_argv[i], ">>"))
            {
                append_filenames[operator_count] = command_argv[i + 1];
                command_argv[i] = NULL;
            }
            else if (0 == strcmp(command_argv[i], "<"))
            {
                input_filenames[operator_count] = command_argv[i + 1];
                command_argv[i] = NULL;
            }
        }

        int exit_status = fork_exec(argv[0], command_argv, output_filenames[0], append_filenames[0], input_filenames[0]);
        for (int i = 0; i < operator_count; i++)
        {
            if ((1 == operator_types[i] && 0 != exit_status) || (2 == operator_types[i] && 0 == exit_status))
            break;

            int subcommand_argv_position = next_subcommand_argv_positions[i];
            exit_status = fork_exec(argv[0], (char* const*)command_argv + subcommand_argv_position, output_filenames[i + 1], append_filenames[i + 1], input_filenames[i + 1]);
        }
    }

    return 0;
}
