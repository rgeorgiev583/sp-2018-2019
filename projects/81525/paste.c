#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    bool s = false;
    bool d = false;
    bool pullThePlug = false;
    char c = 0;
    char delim[1] = "\t";
    int readCount = 0;
    int arg = 1;

    for(; arg<3 && arg<argc; arg++) {
        if(!strcmp(argv[arg], "-s")){
            s = true;
        } else if (argv[arg][0] == '-' && argv[arg][1] == 'd'){
            if(strlen(argv[arg])>2){
                delim[0] = argv[arg][2];
            }
            d = true;
        }
    }

    if(argc == 1) {
        char buffer[BUFSIZ];
        ssize_t read_count;
        while ((read_count = read(STDIN_FILENO, buffer, BUFSIZ)) != 0)
        {
            if (-1 == read_count)
            {
                perror("read");
                exit(3);
            }

            write(STDOUT_FILENO, buffer, read_count);
        }

        return 0;
    }

    if(!s){
        arg--;
    }

    if(!d){
        arg--;
    }

    int *openFiles = (int*) malloc(argc * sizeof(int));

    //OPEN FILES
    for(; arg<argc; arg++) {
        openFiles[arg] = open(argv[arg], O_RDONLY);
        if(!openFiles[arg]) {
            char *errMess = "FILE OPENING ERROR\n";
            write(STDOUT_FILENO, errMess, strlen(errMess));
            exit(1);
        }
    }

    //SPEW CONTENTS
    if(!s) {
        while(!pullThePlug) {
            pullThePlug = true;
            for(int i=1; i<argc; i++) {
                while(openFiles[i] && c != '\n'){
                    pullThePlug = false;
                    readCount = read(openFiles[i], &c, 1);
                    if(!readCount){
                        close(openFiles[i]);
                        openFiles[i] = 0;
                        pullThePlug = true;
                    } else if(c != '\n'){
                        write(STDOUT_FILENO, &c, 1);
                    }
                }
                c = 0;
                if(i+1<argc){
                    write(STDOUT_FILENO, delim, 1);
                }
            }
            if(!pullThePlug) {
                write(STDOUT_FILENO, "\n", 1);
            }
        }

                write(STDOUT_FILENO, "\n", 1);
    } else {
        for(int i=1; i<argc; i++) {
            pullThePlug = false;
            while(openFiles[i]) {
                while(c != '\n' && !pullThePlug){
                    readCount = read(openFiles[i], &c, 1);
                    if(!readCount){
                        close(openFiles[i]);
                        openFiles[i] = 0;
                        pullThePlug = true;
                        write(STDOUT_FILENO, "\n", 1);
                    } else if (c != '\n'){
                        write(STDOUT_FILENO, &c, 1);
                    }
                }
                c = 0;
                if(!pullThePlug){
                    write(STDOUT_FILENO, delim, 1);
                }
            }
            if(openFiles[i]){
                write(STDOUT_FILENO, "\n", 1);
            }
        }
    }

    //CLEANUP
    for(int i=0; i<argc; i++) {
        if(openFiles[i]) {
            close(openFiles[i]);
        }
    }
    free(openFiles);

    return 0;
}
