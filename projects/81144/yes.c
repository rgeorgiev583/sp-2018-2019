

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER 10000

int main( int argc, char** argv ){
    char* list;
    if (argc == 1){
          strcat(list, "y\n");
    }
    else{
        for (int i= 1; i< argc; i++){
            strcat(list, " ");
            strcat(list, argv[i]);
        } 
        strcat(list, "\n");
    }
    
    while (1){
        write (STDOUT_FILENO, list, strlen(list));
    }
    
    return 0;
}
