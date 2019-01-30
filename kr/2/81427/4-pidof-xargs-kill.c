#include <unistd.h>

int main(int argc, char* argv[])
{
 int fd[2];
 pipe(fd);

 int pid_pidof = fork();

 if(pid_pidof == 0)
 {
  close(fd[0]);
  dup2(fd[1], 1);
  argv[0] = "pidof";
  execvp("pidof", argv);
 }

 else{
  int pid_xargs = fork();
  if(pid_xargs == 0)
  {
    close(fd[1]);
    dup2(fd[1],0);
    execlp("xargs", "xargs", "kill");
  }
  else
  {
   waitpid(pid_pidof, NULL,0);
  }
 }
 return 0;
}
