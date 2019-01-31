#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc,char* argv[])
{
	int pipeFile[2];
        pipe(pipeFile);
        int pid = fork();
        if(pid == 0)
        {
                close(pipeFile[0]);
                dup2(pipeFile[1], 1);
                argv[1] = "sort";
                execlp("sort", (char* const) argv, NULL);

        }
	else
	{
		int uniqPid = fork();
        	if(uniqPid == 0)
        	{
                	close(pipeFile[1]);
			dup2(pipeFile[0], 0);
                      	execlp("uniq","uniq", NULL);
        	}

		else
		{
                        int status1;
			close(pipeFile[0]);
                        close(pipeFile[1]);
                        waitpid(pid, &status1, 0);
                        waitpid(uniqPid, &status1, 0);		
		}
	}	

	return 0;
}
