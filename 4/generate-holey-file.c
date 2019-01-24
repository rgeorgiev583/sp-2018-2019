// Употреба: generate-holey-file БРОЙ-ДУПКИ РАЗМЕР-НА-ДУПКА ФАЙЛ

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_FILE_MODE 0644

#define REQUIRED_ARG_COUNT 3
#define TEST_PATTERN "123456:890_HelloWorld\n"
#define TEST_PATTERN_LENGTH sizeof(TEST_PATTERN) - 1

int main(int argc, const char* const* argv)
{
	if (argc < REQUIRED_ARG_COUNT + 1)
		exit(1);

	int hole_count = atoi(argv[1]);
    if (hole_count < 0)
    {
        perror(argv[0]);
        exit(2);
    }

	int hole_size = atoi(argv[2]);
    if (hole_size <= 0)
    {
        perror(argv[0]);
        exit(2);
    }

	int output_fileno = creat(argv[3], DEFAULT_FILE_MODE);
    if (-1 == output_fileno)
    {
        perror("open");
        exit(5);
    }
	
	for (int i = 0; i < hole_count; i++)
	{
		if (-1 == write(output_fileno, TEST_PATTERN, TEST_PATTERN_LENGTH))
        {
            perror("write");
            exit(4);
        }

		if (-1 == lseek(output_fileno, hole_size, SEEK_CUR))
		{
			perror("lseek");
			exit(6);
		}
	}

	return 0;
}