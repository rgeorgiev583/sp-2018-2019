// Употреба: generate-holey-file БРОЙ-ДУПКИ РАЗМЕР-НА-ДУПКА ФАЙЛ

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define DEFAULT_FILE_MODE 0644

#define REQUIRED_ARG_COUNT 3
#define TEST_PATTERN "123456:890_HelloWorld\n"
#define TEST_PATTERN_LENGTH sizeof(TEST_PATTERN) - 1

int main(int argc, const char* const* argv)
{
	if (argc < REQUIRED_ARG_COUNT + 1)
		return 1;

	int gap_count = atoi(argv[1]);
	int gap_size = atoi(argv[2]);
	int output_fileno = creat(argv[3], DEFAULT_FILE_MODE);
	
	for (int i = 0; i < gap_count; i++)
	{
		write(output_fileno, TEST_PATTERN, TEST_PATTERN_LENGTH);
		lseek(output_fileno, gap_size, SEEK_CUR);
	}

	return 0;
}