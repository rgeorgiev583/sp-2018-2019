#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

int main(int argc, char* argv[])
{
	int fd1 = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0755);
	int fd2 = open(argv[5], O_RDONLY);
	dup2(fd1, 1);
	dup2(fd2, 0);
	execlp("tr","tr",argv[1],argv[2]); 
	return 0;
}
