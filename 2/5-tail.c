#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, const char* const* argv)
{
    if (argc < 3)
        return 1;

    int total_count = atoi(argv[1]);
    int input_fileno = open(argv[2], O_RDONLY);
    char buffer;
    size_t current_count = 0;
    while (current_count < total_count && read(input_fileno, &buffer, 1) > 0)
        current_count++;

    while (read(input_fileno, &buffer, 1) > 0)
        write(STDOUT_FILENO, &buffer, 1);

    return 0;
}
