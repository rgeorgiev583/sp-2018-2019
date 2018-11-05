#include <unistd.h>

int main(int argc, char* const* argv)
{
    execvp("cat", argv);
}