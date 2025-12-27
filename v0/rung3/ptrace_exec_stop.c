#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include "syscall_table.h"
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

int status, entering = 1, syscall_count[512] = {0};

waitpid(pid, &status, 0);

if(WIFSTOPPED(status)){

printf("Parent: child stopped at exec under ptrace\n");

}

while(1) {

ptrace(PTRACE_SYSCALL, pid, NULL, NULL);

waitpid(pid, &status, 0);

if(WIFEXITED(status)){

printf("Parent: child exited\n");
break;

}

if(WIFSTOPPED(status)){

struct user_regs_struct regs;
ptrace(PTRACE_GETREGS, pid, NULL, &regs);

if(entering) {

	long sc = regs.orig_rax;
	
	if (sc >= 0 && sc <= 512){
	
	syscall_count[sc]++;
}
	entering = 0;
}

else {
	entering = 1;
}

} //WIFSTOPPED

} //while 

    printf("\n=== Syscll Frequency Summary ===\n");

    for (int i = 0; i < 512; i++){
  
      if (syscall_count[i] > 0 && syscall_names[i] != NULL) {
        printf("%s: %d\n", syscall_names[i], syscall_count[i]);
      }

    }

} // main else
return 0;
}
