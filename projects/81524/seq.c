#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define REQUIRED_ARGS 1
#define SIZE 100

void findSeparator(char* toBeSeparated, char* separator[SIZE])
{
    strcpy(separator, "");
	int i;
	char buff[2];
	int len = strlen(toBeSeparated);
	for (i = 3; i < len - 1; ++i)
	{
		buff[0] = toBeSeparated[i];
		buff[1] = '\0';
		strcat(separator, buff);
	}
}

void writeWithoutAlignment(int start, int diff, int end, char* separator)
{
	while (start <= end)
	{
		printf("%d", start);
		start += diff;
		if (start <= end)
			printf("%s", separator);
	}
}

void writeWithAlignment(int start, int diff, int end, char* separator)
{
	char buff[SIZE];
	sprintf(buff, "%d", end);  // itoa(3) е нестандартна функция!
	int maxN = strlen(buff);
	while (start <= end)
	{
		printf("%0*d", maxN, start);
		start += diff;
		if (start <= end)
			printf("%s", separator);
	}
}


int main(int argc, char* argv[])
{

	if (argc < REQUIRED_ARGS + 1)
	{
		perror(argv[0]);
		return 1;
	}

	int start, diff, end;
	char separator[SIZE];
    strcpy(separator, "\n");

	if (argc == 2)
	{
		start = 1;
		diff = 1;
		end = atoi(argv[1]);
		if (end == 0)
		{
			perror(argv[0]);
			return 2;
		}
		writeWithoutAlignment(start, diff, end, separator);
	}
	else
	{
		char firstArg[SIZE];
		strcpy(firstArg, argv[1]);
		//we have an option(s)
		if (firstArg[0] == '-' && firstArg[1] == 's')
		{
			findSeparator(firstArg, separator);
			char secondArg[SIZE];
			strcpy(secondArg, argv[2]);
			//we have second option
			if (secondArg[0] == '-')
			{
				if (argc == 4)
				{
					start = 1;
					diff = 1;
					end = atoi(argv[3]);
				}
				else if (argc == 5)
				{
					start = atoi(argv[3]);
					diff = 1;
					end = atoi(argv[4]);

				}
				else if (argc == 6)
				{
					start = atoi(argv[3]);
					diff = atoi(argv[4]);
					end = atoi(argv[5]);
				}

				writeWithAlignment(start, diff, end, separator);
			}
			else
			{
				if (argc == 3)
				{
					start = 1;
					diff = 1;
					end = atoi(argv[2]);
				}
				else if (argc == 4)
				{
					start = atoi(argv[2]);
					diff = 1;
					end = atoi(argv[3]);
				}
				else if (argc == 5)
				{
					start = atoi(argv[2]);
					diff = atoi(argv[3]);
					end = atoi(argv[4]);
				}
				writeWithoutAlignment(start, diff, end, separator);
			}
		}
		else if (firstArg[0] == '-' && firstArg[1] == 'w')
		{
			char secondArg[SIZE];
			strcpy(secondArg, argv[2]);
			//we have second option
			if (secondArg[0] == '-')
			{
				findSeparator(secondArg, separator);
				if (argc == 4)
				{
					start = 1;
					diff = 1;
					end = atoi(argv[3]);
				}
				else if (argc == 5)
				{
					start = atoi(argv[3]);
					diff = 1;
					end = atoi(argv[4]);

				}
				else if (argc == 6)
				{
					start = atoi(argv[3]);
					diff = atoi(argv[4]);
					end = atoi(argv[5]);
				}
				writeWithAlignment(start, diff, end, separator);
			}
			else
			{
				if (argc == 3)
				{
					start = 1;
					diff = 1;
					end = atoi(argv[2]);
				}
				else if (argc == 4)
				{
					start = atoi(argv[2]);
					diff = 1;
					end = atoi(argv[3]);
				}
				else if (argc == 5)
				{
					start = atoi(argv[2]);
					diff = atoi(argv[3]);
					end = atoi(argv[4]);
				}

				writeWithAlignment(start, diff, end, separator);
			}
		}
		else
		{
			if (argc == 3)
			{
				start = atoi(argv[1]);
				end = atoi(argv[2]);
				diff = 1;
			}
			else if (argc == 4)
			{
				start = atoi(argv[1]);
				diff = atoi(argv[2]);
				end = atoi(argv[3]);
			}

			writeWithoutAlignment(start, diff, end, separator);
		}
	}
	return 0;
}
