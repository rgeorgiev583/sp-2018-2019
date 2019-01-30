#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

int tabsFiles(char* argv[], int n, int fileNumber)
{
	char spaces[100] = "";
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
		if ((fd = open(argv[i], O_RDONLY))== -1)
		{
			perror(argv[i]);
			return 2;
		}

		while ((size = read(fd, &buff, 1)) > 0)
		{
			if (buff == '\t')
				write(1, spaces, n);
			else
				write(1, &buff, size);
		}

		if (close(fd) == -1)
		{
			perror(argv[i]);
			return 3;
		}
	}
	return 0;
}

int tabsPrompt(int n)
{
	char spaces[100] = "";
	char buff;

	int i = 0;
	while (i < n)
	{
		strcat(spaces, " ");
		i++;
	}
	strcat(spaces, "\0");

	int size;
	while ((size = read(0, &buff, 1)) > 0)
	{
		if (buff == '\t')
			write(1, spaces, n);
		else
			write(1, &buff, size);
	}

	return 0;
}

bool isLightSpace(char previous)
{
	return previous == '\t' || previous == '\n' || previous == '\r' || previous == ' ';
}

int tabsFilesSpace(char* argv[], int n, int fileNumber)
{
	char spaces[100] = "";
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
	int j = 0;
	for (j; j < fileNumber; j++)
	{
		if ((fd = open(argv[j], O_RDONLY)) == -1)
		{
			perror(argv[j]);
			return 2;
		}

		while ((size = read(fd, &buff, 1)) > 0)
		{
			if (buff == '\t' && isLightSpace(previous))
				write(1, spaces, n);
			else
				write(1, &buff, size);
			previous = buff;
		}

		if (close(fd) == -1)
		{
			perror(argv[j]);
			return 3;
		}
	}

	return 0;
}

int tabsPromptSpace(int n)
{
	char spaces[100] = "";
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
	while ((size = read(0, &buff, 1)) > 0)
	{
		if (buff == '\t' && isLightSpace(previous))
			write(1, spaces, n);
		else
			write(1, &buff, size);
		previous = buff;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	if (argc < 1)
	{
		perror(argv[0]);
		return 1;
	}

    int n = 8;

	if(argc > 1)
    {
        if (argv[1][0] == '-' && argv[1][1] == 'i')
        {
            if(argc>2)
            {
                if (argv[2][0] == '-')
                {
                    n = atoi(argv[2] + 2);
                    if (argc > 3)
                        return tabsFilesSpace(argv + 3, n, argc - 3);

                    return tabsPromptSpace(n);
                }

                return tabsFilesSpace(argv + 2, n, argc - 2);
            }

            return tabsPromptSpace(n);
        }

        if (argv[1][0] == '-' && argv[1][1] == 't')
        {
            n = atoi(argv[1] + 2);
            if (argc > 2)
            {
                if (argv[2][0] == '-')
                {
                    if (argc > 3)
                        return tabsFilesSpace(argv + 3, n, argc - 3);

                    return tabsPromptSpace(n);
                }

                return tabsFiles(argv + 2, n, argc - 2);
            }

            return tabsPrompt(n);
        }

		return tabsFiles(argv + 1, n, argc - 1);
    }

	return tabsPrompt(n);
}
