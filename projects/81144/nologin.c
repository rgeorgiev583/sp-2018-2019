#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

#define DEFAULT_MSG "This account is currently not available.\n"
#define DEFAULT_MSG_FILE "/etc/nologin.txt"
#define MSG_SIZE 1000

int main(void){

    char* message;
    int input_fileno = open(DEFAULT_MSG_FILE, O_RDONLY);
    if ( input_fileno == -1){
         perror("open");
         message = DEFAULT_MSG;
         
    }
    else {
        ssize_t length;
        message = (char *) realloc (message,MSG_SIZE);
        if((length = read(input_fileno, message, MSG_SIZE))==-1){
            perror("read");
            exit(1);
       }
    }
    write(STDOUT_FILENO, message, strlen(message));   
    return 1;
}
