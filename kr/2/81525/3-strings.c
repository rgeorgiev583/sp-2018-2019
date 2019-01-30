#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>  // променено с цел компилация

#define MAX_STD_FILENO 2

bool isSym(char c)
{
	return c>=32 && c<=126;
}

int main(int argc, char const* const* argv)
{
	char buffy;
	char hvf[4];	
	int count = 0;
	int br = 0;

	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if (-1 == open(argv[i], O_RDONLY))
			{
                		perror(argv[0]);
                		return 1;
			}
		}

		for(int i=1; i<argc; i++)
		{
			while ((count = read(MAX_STD_FILENO + i, &buffy, 1)) != 0)  // променено
			{
              			if (-1 == count)
				{
                    			perror(argv[0]);
					return 2;
				}

				if (isSym(buffy))
				{
					if(br<4)
					{
						hvf[br] = buffy;
						br++;
					} else if(br==4) {
						write(STDOUT_FILENO, hvf, 4);
						br++;
					} else if (br>4) {
						write(STDOUT_FILENO, &buffy, 1);  // променено
					}	
				} else {
					br = 0;
				}

                	}

		}
	} else {
		while ((count = read(STDIN_FILENO, &buffy, 1)) != 0)  // променено
                        {
                                if (-1 == count)
                                {
                                        perror(argv[0]);
                                        return 2;
                                }

                                if (isSym(buffy))
                                {
                 	                if(br<4)
                                        {
                                   	        hvf[br] = buffy;
                                        	br++;
                                	} else if(br==4) {
                                        	write(STDOUT_FILENO, hvf, 4);
						br++;
                                	} else if (br>4) {
                                       		write(STDOUT_FILENO, &buffy, 1);  // променено
                   			}
                	        } else {
                        	        br = 0;
				}
			}
	}
}
