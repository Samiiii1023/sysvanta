#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

pid_t pid;

printf("Before fork\n");

pid = fork();

if(pid < 0){
perror("fork failed");
exit(1);
}
if(pid == 0){
printf("Parent: PID=%d, Child: PID=%d\n", getppid(), getpid());
sleep(3);
printf("Child exiting\n");
exit(42);
}
else{
int status;
printf("Parent: PID=%d, Child: PID=%d\n", getpid(), pid);
wait(&status);
if(WIFEXITED(status)){
printf("Parent: Child exited with status %d\n", WEXITSTATUS(status));
 }
}
return 0;

}
