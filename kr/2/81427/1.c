#include<unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
 int fn = 81427;
 int N = atoi(argv[1]);
 int symLen = N + fn%23 + 4;

 int fd1 = open(argv[1],O_RDONLY);
 int fd2 = open(argv[2], O_WRONLY);

 lseek(fd2, symLen, SEEK_END);

 char buff[225];
 int count;

 while( (count = read(fd1, buff, 225)) != 0 )
 {
  write(fd2, buff, count);
 }
}
