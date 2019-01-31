#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc, char** argv) {
	if(argc < 5) {
		perror(argv[0]);
		return 1;
	}

	int file1, file2;

	if((file1 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR)) < 0) {
		perror(argv[0]);
		return 1;
	}
	
	if((file2 = open(argv[4], O_RDONLY)) < 0) {
		perror(argv[0]);
		return 1;
	}

	if(dup2(file1, 1) == -1) {
		perror(argv[0]);
		return 1;
	}

	if(dup2(file2, 0) == -1) {
		perror(argv[0]);
		return 1;
	}

	close(file1);
	close(file2);

	execlp("tr", "tr", argv[1], argv[2], NULL);

	perror(argv[0]);

	return 1;
}
