#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define required_args 3

int main(int argc, char* argv[])
{
	if(argc < required_args + 1)
		return 1;
	
	int fnMod = 81524%29;
	int n = atoi(argv[1]);
	int offset = n + fnMod + 3;
	
	int fd1 = open(argv[2], O_RDONLY);
	int fd2 = open(argv[3], O_WRONLY);
	if(lseek(fd2, offset, SEEK_END))	
	{
		int size;
		char buf;
		while((size=read(fd1,&buf,1))>0)
		{
			write(fd2,&buf, size);
		}
	}
	else
	{
		close(fd1);
		close(fd2);
		return 2;
	}	
	close(fd1);
	close(fd2);
	return 0;
}
