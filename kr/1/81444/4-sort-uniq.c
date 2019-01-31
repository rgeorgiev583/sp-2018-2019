#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char** argv) {
	int pipefd[2];

	if(pipe(pipefd) == -1) {
		perror(argv[0]);
		return 1;
	}
	
	pid_t sort_pid = fork();
	
	if(sort_pid == -1) {
		perror(argv[0]);
		return 1;
	}
	else if(sort_pid == 0) {
		if(close(pipefd[0]) == -1) {
			perror(argv[0]);
			return 1;
		}

		if(dup2(pipefd[1], 1) == -1) {
			perror(argv[0]);
			return 1;
		}

		argv[0] = "sort";
		execvp("sort", argv);
		
		perror(argv[0]);
		return 1;
	}

	pid_t uniq_pid = fork();
	if(uniq_pid == -1) {
		perror(argv[0]);
		return 1;
	}
	else if(uniq_pid == 0) {
		if(close(pipefd[1]) == -1) {
			perror(argv[0]);
			return 1;
		}
		
		if(dup2(pipefd[0], 0) == -1) {
			perror(argv[0]);
			return 1;
		}

		execlp("uniq", "uniq", NULL);

		perror(argv[0]);
		return 1;
	}

	if(close(pipefd[0]) == -1) {
		perror(argv[0]);
		return 1;
	}

	if(close(pipefd[1]) == -1) {
		perror(argv[0]);
		return 1;
	}

	waitpid(sort_pid, NULL, 0);

	int status;
	if(waitpid(uniq_pid, &status, 0) == -1) {
		perror(argv[0]);
	}

	return WEXITSTATUS(status);
}
