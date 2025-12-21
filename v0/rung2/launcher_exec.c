#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

pid_t pid;

printf("Launcher: Starting\n");

pid = fork();

if (pid < 0){

perror("fork failed");
exit(1);

}

if (pid == 0){

//child process

printf("Child: about to exec the target program.\n");

execl("./v0/rung2/target", "target", NULL); //execl let specify args as list of str, NULL ptr must terminate the arg list, so it can locate end of the list

//only runs if exec fails

perror("exec failed");
exit(1); 

}

else {

//parent process

int status;

printf("Parent: waiting for child PID=%d\n", pid);

waitpid(pid, &status, 0);

printf("Parent: child finished\n");

}
return 0;

}

