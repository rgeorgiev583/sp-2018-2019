#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main (int argc, const char* const* argv){
    if (argc <2){
    
    }   
   
    char* pid_dir;
    pid_dir = (char *) malloc(20);
    strcpy(pid_dir,"/proc/");
    strcat(pid_dir,argv[1]);
    strcat(pid_dir, "/cwd");
    int dir_name_file = open("./name", O_CREAT | O_TRUNC |O_RDWR);
        
    switch(fork()){
        case -1:
            perror("fork");
            exit(9);
        case 0:
            dup2(STDOUT_FILENO,dir_name_file); 
            if (execlp("ls","ls","-l",pid_dir, NULL)==-1){
                perror("exec");
                exit(8);
           
            }
    }
   
    close(dir_name_file);
    
    return 0;
}

