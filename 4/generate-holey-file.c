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
		return 1;

	int gap_count = atoi(argv[1]);
    if (gap_count < 0)
    {
        perror(argv[0]);
        return 2;
    }

	int gap_size = atoi(argv[2]);
    if (gap_size < 0)
    {
        perror(argv[0]);
        return 2;
    }

	int output_fileno = creat(argv[3], DEFAULT_FILE_MODE);
    if (-1 == output_fileno)
    {
        perror(argv[0]);
        return 5;
    }
	
	for (int i = 0; i < gap_count; i++)
	{
		if (-1 == write(output_fileno, TEST_PATTERN, TEST_PATTERN_LENGTH))
        {
            perror(argv[0]);
            return 4;
        }

		if (-1 == lseek(output_fileno, gap_size, SEEK_CUR))
		{
			perror(argv[0]);
			return 6;
		}
	}

	return 0;
}