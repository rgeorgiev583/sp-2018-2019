#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>

#define NUMBER_OF_ARGS 0



int size(char* word) {
    int result = 0;
    int index = 0;
    while (word[index]!='\0') {
        result++;
        index++;
    }
    return result;
}
int main(int argc, const char* const* argv) {

    if ( argc <=NUMBER_OF_ARGS+1) {
       for (;;) {
            write(STDOUT_FILENO, "y\n", 2);
        }
    } else {
        int i;
        for (;;) {
            for (i = 1; i<argc; i++) {
                write(STDOUT_FILENO, argv[i], size(argv[i]));
                write(STDOUT_FILENO, " ", 1);
            }
            write(STDOUT_FILENO, "\n", 1);
        }
    }
    return 0;
}
