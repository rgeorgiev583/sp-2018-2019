#include<unistd.h>
#include<fcntl.h>

void izvedi(int fd)
{
 int curlen = 0;
 int lastpos = 0;
 char buf[225];
 char writebuf[225];
 int count = 0;

 while( (count=read(fd,buf,225)) != 0 )
 {
   int i = 0;
   while(i<count)
   {
     // read till all is fine (char is visible)
     if( (buf[i]>=' ') && (buf[i]<='~') )
     {
       i++;curlen++;
     }
     else
     {
       // if we get to a 'reset' symbol
       int j = 0;
       while(j<curlen && j < i)
       {
        writebuf[j] = buf[lastpos+j];
        j++;
       }
       // print currenly buffered output
       if(curlen>0) write(1,writebuf,j);
       curlen = 0;
       lastpos = i;
     }
   }
   if(curlen > 0)
   {
     // we got to end of reading buffer and did not reset - then print what we have until now,
     // but save the curlen for the new buffered input
     int j = 0;
     while(j<curlen)
     {
      writebuf[j]=buf[lastpos+j];
      j++;
     }
     write(1,writebuf,curlen);
     lastpos=0;
   }
 }
}

int main(int argc,char* argv[])
{
 int fd = 0;
 if(argc==1) izvedi(fd);
 else
 {
  int i = 1;
  while(i < argc)
  {
   if(0 == fork())
   {
    int fdread = open(argv[i], O_RDONLY);

    izvedi(fdread);
   }
   else i++;
  }
 }
}
