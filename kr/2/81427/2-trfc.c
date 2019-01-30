#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

int main(int argc, char* argv[])
{
 int i = 3;

 if(argc==3)
 {
  execlp("trfc", "trfc", argv[2], argv[1]);
 }

 while(i < argc)
 {
  if(0 == fork())
  {
   int fd = open(argv[i], O_RDONLY);
   dup2(0, fd);
   execlp("trfc", "trfc", argv[2], argv[1]);
  }
  else i++;
 }

 return 0;
}
