#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define required_args 4
int main(int argc, char* argv[])
{
	if(argc < required_args + 1)
		return 1;
	
	int fd1 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	int fd2 = open(argv[4], O_RDONLY);
	
	dup2(fd1, STDOUT_FILENO);
	dup2(fd2, STDIN_FILENO);
	execlp("tr", "tr", argv[1], argv[2],NULL);
	
	return 0;
}
