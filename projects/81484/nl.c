#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define NEWLINE_CHARACTER '\n'

int out(int n, int fd, int ninc,int form,int strl,char * str,int wn)
{
	 int j;
	 char buffer;
	 char numbuffer[100];	
        
		if(form==1)
        j = sprintf(numbuffer, "%-*d",wn, n);
	    if(form==2)
		j = sprintf(numbuffer, "%*d",wn, n);
	    if(form==3)
        j = sprintf(numbuffer, "%0*d",wn, n);		
	
	write(STDOUT_FILENO, numbuffer, j);
	write(STDOUT_FILENO, str,strl);
		
    while (read(fd, &buffer, 1) > 0)
    {
        if (NEWLINE_CHARACTER != buffer)
		{
		write(STDOUT_FILENO, &buffer,1);
		}
		
        if (NEWLINE_CHARACTER == buffer)
		{
	    write(STDOUT_FILENO, &buffer,1);
        n=n+ninc;
		int check=read(fd,&buffer,1);
			while(check>0&&NEWLINE_CHARACTER == buffer)
			{
				write(STDOUT_FILENO, &buffer,1);
				check=read(fd,&buffer,1);
			}
		  if(check>0)
		    {			
		     if(form==1)
             j = sprintf(numbuffer, "%-*d",wn, n);
	         if(form==2)
		     j = sprintf(numbuffer, "%*d",wn, n);
	         if(form==3)
             j = sprintf(numbuffer, "%0*d",wn, n);	
	         write(STDOUT_FILENO, numbuffer, j);
	         write(STDOUT_FILENO, str,strl);
		     write(STDOUT_FILENO, &buffer,1);
		    }
			if(check<=0) return n;
		}
    }
	n=n+ninc;
	buffer=NEWLINE_CHARACTER;
	write(STDOUT_FILENO, &buffer,1);
	
	return n;
}



int main(int argc, const char* const* argv)
{
	int n=1;
	int ninc =1;
	int form=2;
	//1=ln 2=rn 3=rz
	char str[50] =" ";
	int strl =1;
	char strprom[3];
	int strpl;
	int wn=6;
	int f=0;
	
	int i=1;
	
	while(i<argc&&!strncmp(argv[i],"-",1))
	{
	if (i<argc&&!strncmp(argv[i], "-i",2))
		 {
			 strpl=strlen(argv[i]);
		if(strpl>5)
		{
				 perror(argv[0]);
				 return 1;
			 }
			 strcpy(strprom,argv[i]+2);
			 ninc=atoi(strprom);
			 if(ninc<=0||ninc>100)
			 {
				 perror(argv[0]);
				 return 1;
			 }
		 
		 }
		
	if (i<argc&&!strncmp(argv[i], "-n",2))	
	{
		strpl=strlen(argv[i]);
		if(strpl>5)
		{
				 perror(argv[0]);
				 return 1;
			 }
		strcpy(strprom,argv[i]+2);
		if (!strcmp(strprom, "ln"))
			form=1;
		if (!strcmp(strprom, "rn"))
			form=2;
		if (!strcmp(strprom, "rz"))
			form=3;
	}
	
	if (i<argc&&!strncmp(argv[i], "-s",2))	
	{
		strl=strlen(argv[i]);
		if(strl>52)
		{
				 perror(argv[0]);
				 return 1;
			 }
		strcpy(str,argv[i]+2);
		strl=strlen(str);
	}
	
	if (i<argc&&!strncmp(argv[i], "-w",2))	
	{
		 strpl=strlen(argv[i]);
		if(strpl>5)
		{
				 perror(argv[0]);
				 return 1;
			 }
		strcpy(strprom,argv[i]+2);
		 wn=atoi(strprom);
			 if(wn<=1||wn>100)
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
		n=out(n,fd,ninc,form,strl,str,wn);
		
	}
	
	if(f==0)
	{
		n=out(n,STDIN_FILENO,ninc,form,strl,str,wn);
	}
		
	
	return 0;
}