#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#define SPACE_CHARACTER ' '

void out(int n, int aflag, int fd)
{
	 int skip=0;
	 char buffer;
	 char prev='\n';
	 char * placement="\\t";
	 char space=' ';
	 
	  while (read(fd, &buffer, 1) > 0)
    {
        if (SPACE_CHARACTER!= buffer)
		{
		write(STDOUT_FILENO, &buffer,1);
		prev=buffer;
		}
		
        if (SPACE_CHARACTER== buffer)
		{
			if(aflag==0 && prev!='\n'&& prev!='\t'&& prev!='\r' && prev!=' ')
			{
				if(SPACE_CHARACTER==prev)
				write(STDOUT_FILENO, &prev,1);	
				write(STDOUT_FILENO, &buffer,1);
				prev=buffer;
				skip=1;
			}
			else
			{
			    int i=1;
				if(SPACE_CHARACTER==prev&&skip==0)
				i=2;
			    else
					skip=0;
				int check=read(fd,&buffer,1);
				
				while(i<n&& check>0 && SPACE_CHARACTER== buffer)
				{
					i++;
					check=read(fd,&buffer,1);
				}
			    if(i==n) 
			    {
				write(STDOUT_FILENO, placement,3);
				if(SPACE_CHARACTER!= buffer)
				write(STDOUT_FILENO, &buffer,1);
			    }
			    if(i<n)
			    {
				for(int j=0; j<i; j++)
					write(STDOUT_FILENO, &space,1);
				if(SPACE_CHARACTER!= buffer)
			     write(STDOUT_FILENO, &buffer,1); 
			     }
			if(check>0) 
				prev=buffer;
			if(check<=0) return;
			
			}
		}
	}
}


int main(int argc, const char* const* argv)
{
	char str[50];
	int aflag=0;
	int n=8;
	int f=0;
	
	int i=1;
	
	while(i<argc&&!strncmp(argv[i],"-",1))
	{
	if(i<argc&&!strcmp(argv[i], "-a"))	
	{
		aflag=1;
	}
	
	if (i<argc&&!strncmp(argv[i], "-t",2))	
	{
		strcpy(str,argv[i]+2);
		 n=atoi(str);
			 if(n<=0)
			 {
				 perror(argv[0]);
				 return 1;
			 }
		
	}
	i++;
	}
	
	for (int j = i; j < argc; j++)
	{
		f=1;
		int fd=open(argv[j], O_RDONLY);
		if(-1==fd)
		    {
                perror(argv[0]);
                return 1;
            }
		out(n,aflag,fd);
		
	}
	
	if(f==0)
	{
		out(n,aflag,STDIN_FILENO);
	}
		
	
	return 0;
}