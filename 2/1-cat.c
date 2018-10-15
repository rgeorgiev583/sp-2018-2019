#include <unistd.h>

#define BUFFER_SIZE 1000

int main()
{
    char buffer[BUFFER_SIZE];
    size_t count;
    while ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
        write(STDOUT_FILENO, buffer, count);

    return 0;
}
