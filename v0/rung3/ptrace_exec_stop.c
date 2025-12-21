#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>

int main(){

pid_t pid = fork();

if (pid == 0){
//child process
ptrace(PTRACE_TRACEME, 0, NULL, NULL);
execl("./rung3/target", "target", NULL);
perror ("exec failed");
exit(1);
}

else{
//parent process

int status;

waitpid(pid, &status, 0);

if(WIFSTOPPED(status)){

printf("Parent: child stopped at exec under ptrace\n");
ptrace(PTRACE_CONT, pid, NULL, NULL);
}

waitpid(pid, &status, 0);
printf("Parent: child exited\n");

}
return 0;
}
