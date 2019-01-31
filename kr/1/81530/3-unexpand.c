#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int counter = 0;
	char buff;
	char tab = '\t';
	char tmpBuff[8];
	int size;
	int tmpSize =0;
	if(argc>0)
	{
		int i = 1;
		for(i; i<argc; i++)
		{
			int fd = open(argv[i], O_RDONLY);
			while((size=read(fd,&buff,1))>0)
        	        {
                	        if(buff == ' ')
                       		{
                        	        counter++;
                              	 	tmpBuff[tmpSize] = buff;
                         	   	tmpSize++;
                        	}
                        	else
                        	{
                                	if(counter==8)
                                	{	
                                        	write(1,&tab, 1);
                                        	counter =0;
                                        	tmpSize=0;
                                	}
                                	else if(counter> 0)
                                	{
                                        	write(1,tmpBuff, tmpSize);
                                        	counter =0;
                                        	tmpSize = 0;
                                	}
                                	else if(counter==0)
                                	{
                                        	write(1,&buff, size);
                                	}
                        	}
                	}

		}	
	}
	else
	{	
		while((size=read(0,&buff,1))>0)
		{
			if(buff == ' ')
			{
				counter++;
				tmpBuff[tmpSize] = buff;
				tmpSize++;
			}
			else
			{
				if(counter==8)
				{
					write(1,&tab, 1);
					counter =0;
					tmpSize=0;
				}
				else if(counter> 0)
				{
					write(1,tmpBuff, tmpSize);
					counter =0;
					tmpSize = 0;
				}
				else if(counter==0)
				{
					write(1,&buff, size);
				}
			}	
		}

	}		
	return 0;
}
