#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[])
{
	char bufSpace[] = "        ";
	if(argc > 0)
	{	
		int i;
		for(i=1; i<=argc; i++)
		{
			int fd = open(argv[i], O_RDONLY);
			int size;
			char buf[8];
			while((size=read(fd,buf,8))>0)
			{
				if(strcmp(buf, bufSpace) == 0)
					write(STDOUT_FILENO, "\t",1);
				else
					write(STDOUT_FILENO, buf, 8);
			}
			close(fd);
		}	
	}
	else
	{
		int size;
		char buf[8];
		while((size=read(STDIN_FILENO, buf, 8))>0)
		{
			if(strcmp(buf,bufSpace) == 0)
				write(STDOUT_FILENO, "\t",1);
			else
				write(STDOUT_FILENO, buf, 8);
		}
	}
	
	return 0;
}

