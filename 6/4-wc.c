#include <unistd.h>
#include <string.h>

#define REQUIRED_ARG_COUNT 1

int main(int argc, char** argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    if (strcmp(argv[1], "chars"))
        argv[1] = "-c";
    else if (strcmp(argv[1], "words"))
        argv[1] = "-w";
    else if (strcmp(argv[1], "lines"))
        argv[1] = "-l";

    execvp("wc", (char* const*)argv);
}