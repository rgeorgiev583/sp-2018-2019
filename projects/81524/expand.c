#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

#define REQUIRED_ARGS 0
#define SIZE 200

int convertTabsInFiles(char* argv[], int n, int fileNumber)
{
	char spaces[SIZE];
	strcpy(spaces,"");

	char buff;
	int i = 0;
	while (i < n)
	{
		strcat(spaces, " ");
		i++;
	}
	strcat(spaces, "\0");

	int fd, size;
	i = 0;
	for (i; i < fileNumber; i++)
	{
		fd = open(argv[i], O_RDONLY);
		if (fd == -1)
		{
			perror(argv[i]);
			return 2;
		}

		while ((size = read(fd, &buff, 1)) > 0)
		{
			if (buff == '\t')
            {
                if(write(STDOUT_FILENO, spaces, n)==-1)
                    return 3;
            }
			else
            {
                if(write(STDOUT_FILENO, &buff, size)==-1)
                    return 4;
            }
		}

		if (close(fd) == -1)
		{
			perror(argv[i]);
			return 5;
		}
	}
	return 0;
}

int convertTabsInPrompt(int n)
{
	char spaces[SIZE];
	strcpy(spaces,"");
	char buff;

	int i = 0;
	while (i < n)
	{
		strcat(spaces, " ");
		i++;
	}
	strcat(spaces, "\0");

	int size;

	while ((size = read(STDIN_FILENO, &buff, 1)) > 0)
	{
		if (buff == '\t')
        {
            if(write(STDOUT_FILENO, spaces, n)==-1)
                return 3;
        }
		else
        {
            if(write(STDOUT_FILENO, &buff, size)==-1)
                    return 4;
        }
	}

	return 0;
}

bool isLightSpace(char previous)
{
	return previous == '\t' || previous == '\n' || previous == '\r' || previous == ' ';
}

int convertTabsInFilesWithLightSpace(char* argv[], int n, int fileNumber)
{
	char spaces[SIZE];
	strcpy(spaces,"");
	char buff;
	char previous = "";

	int i = 0;
	while (i < n)
	{
		strcat(spaces, " ");
		i++;
	}
	strcat(spaces, "\0");

	int fd, size;
	i = 0;
	for (i; i < fileNumber; i++)
	{
		fd = open(argv[i], O_RDONLY);
		if (fd == -1)
		{
			perror(argv[i]);
			return 2;
		}

		while ((size = read(fd, &buff, 1)) > 0)
		{
			if (buff == '\t' && isLightSpace(previous))
            {
                if(write(STDOUT_FILENO, spaces, n)==-1)
                    return 3;
            }
			else
            {
                if(write(STDOUT_FILENO, &buff, size)==-1)
                    return 4;
            }
			previous = buff;
		}

		if (close(fd) == -1)
		{
			perror(argv[i]);
			return 5;
		}
	}

	return 0;
}

int convertTabsInPromptWithLightSpace(int n)
{
	char spaces[SIZE];
	strcpy(spaces,"");
	char buff;
	char previous = "";

	int i = 0;
	while (i < n)
	{
		strcat(spaces, " ");
		i++;
	}
	strcat(spaces, "\0");

	int size;
	i = 0;

	while ((size = read(STDIN_FILENO, &buff, 1)) > 0)
	{
		if (buff == '\t' && isLightSpace(previous))
        {
            if(write(STDOUT_FILENO, spaces, n)==-1)
                return 3;
        }
		else
        {
            if(write(STDOUT_FILENO, &buff, size)==-1)
                return 4;
        }
		previous = buff;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	if (argc < REQUIRED_ARGS + 1)
	{
		perror(argv[0]);
		return 1;
	}

    int n = 8;

	if(argc > 1)
    {
        char firstArg[SIZE];
        strcpy(firstArg, argv[1]);
        if (firstArg[0] == '-' && firstArg[1] == 'i')
        {
            if(argc>2)
            {
                char secondArg[SIZE];
                strcpy(secondArg, argv[2]);
                if (secondArg[0] == '-')
                {
                    n = atoi(secondArg + 2);

                    if (argc > 3)
                        return convertTabsInFilesWithLightSpace(argv + 3, n, argc - 3);

                    return convertTabsInPromptWithLightSpace(n);
                }

                return convertTabsInFilesWithLightSpace(argv + 2, n, argc - 2);
            }

            return convertTabsInPromptWithLightSpace(n);
        }

        if (firstArg[0] == '-' && firstArg[1] == 't')
        {
            n = atoi(firstArg + 2);
            if (argc > 2)
            {
                char secondArg[SIZE];
                strcpy(secondArg, argv[2]);
                if (secondArg[0] == '-')
                {
                    if (argc > 3)
                        return convertTabsInFilesWithLightSpace(argv + 3, n, argc - 3);

                    return convertTabsInPromptWithLightSpace(n);
                }

                return convertTabsInFiles(argv + 2, n, argc - 2);
            }

            return convertTabsInPrompt(n);
        }

		return convertTabsInFiles(argv + 1, n, argc - 1);
    }

	return convertTabsInPrompt(n);
}
