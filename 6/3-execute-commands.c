#include <unistd.h>

int main(int argc, char* const* argv)
{
    for (int i = 1; i < argc; i++)
        if (!fork())
            execlp(argv[i], argv[i], NULL);

    return 0;
}