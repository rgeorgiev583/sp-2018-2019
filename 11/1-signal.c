#include <signal.h>
#include <stdbool.h>

int main(void)
{
    signal(SIGINT, SIG_IGN);

    while (true);

    return 0;
}