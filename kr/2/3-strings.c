#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MIN_SEQUENCE_LENGTH 4

static bool is_printable(char ch)
{
    return ch >= ' ' && ch <= '~';
}

static void strings(int input_fileno)
{
    ssize_t sequence_length = 0;
    char sequence_beginning[MIN_SEQUENCE_LENGTH];

    char buffer;
    ssize_t read_result;
    while ((read_result = read(input_fileno, &buffer, 1)) > 0)
    {
        if (-1 == read_result)
        {
            perror("read");
            exit(5);
        }

        if (is_printable(buffer))
        {
            if (sequence_length < MIN_SEQUENCE_LENGTH)
                sequence_beginning[sequence_length] = buffer;
            else if (sequence_length == MIN_SEQUENCE_LENGTH)
                write(STDOUT_FILENO, sequence_beginning, MIN_SEQUENCE_LENGTH);
            else
                write(STDOUT_FILENO, &buffer, 1);

            sequence_length++;
        }
        else
            sequence_length = 0;
    }
}

int main(int argc, const char* const* argv)
{
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            int input_fileno = open(argv[i], O_RDONLY);
            if (-1 == input_fileno)
            {
                perror("open");
                exit(4);
            }

            strings(input_fileno);
        }
    }
    else
        strings(STDIN_FILENO);

    return 0;
}
