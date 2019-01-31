#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	int fd[2];
	pipe(fd);
	int sort_pid = fork();
	switch(sort_pid)
	{
	case -1:
	{
		close(fd[0]);
		close(fd[1]);
		return 1;
	}
	case 0:
	{
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		argv[0] = "sort";
		execvp("sort",argv);	
	}
	}
	
	int uniq_pid = fork();
	switch(uniq_pid)
	{
	case -1:
	{
		close(fd[0]);
		close(fd[1]);
		return 2;
	}
	case 0:
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		execlp("uniq", "uniq", NULL);
	}
	}
	close(fd[0]);
	close(fd[1]);
	
	int status1, status2;
	waitpid(sort_pid,&status1,0);
	waitpid(uniq_pid, &status2,0);
	
	return WEXITSTATUS(status2);
}
