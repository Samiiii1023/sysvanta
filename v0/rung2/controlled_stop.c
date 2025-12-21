#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(){

pid_t pid;

pid = fork();

if(pid < 0){

perror("fork failed");
exit(1);

}

if(pid == 0){

//child process

printf("Child PID: %d stopping itself\n", getpid());
raise(SIGSTOP);

    exit(0);
  }

else {
  
    //parent process
  
  int status;
  
  waitpid(pid, &status, WUNTRACED);

  if (WIFSTOPPED(status)){
  
          printf("Child stopped by the signal %d\n", WSTOPSIG(status));
          
          }

    kill(pid, SIGCONT);

    waitpid(pid, &status, 0);
    printf("Child exited with %d\n", WEXITSTATUS(status));

  }

return 0;

}
