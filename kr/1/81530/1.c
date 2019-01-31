#include<unistd.h>
#include<stdio.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);
	int fd1 = open(argv[2],O_RDONLY);
	int fd2 = open(argv[3], O_WRONLY);
	int size = n + 81530%29 + 3;
	
	if((lseek(fd2, size, SEEK_END))>0)
	{	
		int s;
		char buff[100];
		while((s=read(fd1, buff, 100))>0)
		{
			write(fd2,buff, size);
		}
	}
	close(fd1);
	close(fd2);
	return 0;
}
