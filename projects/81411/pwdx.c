#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>

#define NUMBER_OF_ARGS 1

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

    if (argc<NUMBER_OF_ARGS) {
        return 1;
    }

    int i=1;
    for (i; i<argc; ++i) {
        char proc [50]  = "proc/";
        strcat(proc, argv[i]);
        strcat(proc, "/cwd");
        int fd = open(proc, O_RDONLY);
        if (fd == -1) {
            perror(argv[1]);
            return 2;
        }
        int count;
        char buffer;
        while ((count=read(fd, &buffer, 1))!=0) {
            write(STDOUT_FILENO, &buffer, 1);
        }
        write(STDOUT_FILENO, "/n", 1);
    }


return 0;
}
