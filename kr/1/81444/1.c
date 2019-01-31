#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>

#define bufferSize 100

int main(int argc, char** argv) {
	if(argc < 4) {
		perror(argv[0]);
		return 1;
	}

	int n = atoi(argv[1]);
	
	int file1, file2;

	if((file1 = open(argv[2], O_RDONLY)) < 0) {
		perror(argv[0]);
		return 1;
	}

	if((file2 = open(argv[3], O_WRONLY)) < 0) {
		perror(argv[0]);
		return 1;
	}

	int offset = n + 81444 % 29 + 3;
	
	printf("Offset is: %d\n", offset);

	if(lseek(file2, offset, SEEK_END) == -1) {
		perror(argv[0]);
		return 1;
	}
	
	char buffer[bufferSize];
	int bytesRead;

	while((bytesRead = read(file1, buffer, bufferSize)) > 0) {
		write(file2, buffer, bytesRead);
	}

	close(file1);
	close(file2);

	return 0;
}
