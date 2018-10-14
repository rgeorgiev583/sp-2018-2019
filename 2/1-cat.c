#include <unistd.h>

#define BUFFER_SIZE 1000

int main()
{
    char buffer[BUFFER_SIZE];
    size_t count;
    while (count = read(0, buffer, BUFFER_SIZE))
        write(1, buffer, count);

    return 0;
}
