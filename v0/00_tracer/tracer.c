#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/user.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

#include "syscall_table.h"

#define MAX_SYSCALLS 10000
#define MAX_BIGRAMS 1000

int main(){

int syscall_stream[MAX_SYSCALLS];
int sc_index = 0;

typedef struct {

int a;
int b;
int count;

} bigram_t;

bigram_t bigrams[MAX_BIGRAMS];
int bigram_count = 0;

pid_t pid = fork();

if( pid == 0 ){ // child process
	
	ptrace(PTRACE_TRACEME, 0, NULL, NULL);
	execl("./target", "target", NULL);
	perror("exec failed.\n");
	exit(1);

	}

else { // parent process

	int status, trace_active = 1, enter = 1, syscall_count[512] = {0};

	waitpid(pid, &status, 0);

	if(WIFSTOPPED(status)){	
		printf("Parent :  Child stopped at exec under ptrace\n");
		
	}

	while(trace_active){
		
	ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
	
	waitpid(pid, &status, 0);
 
	if(WIFEXITED(status)){
	
	printf("Parent : Child Exited.\n");
	break;	
		}

	if(WIFSTOPPED(status)){

	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, pid, NULL, &regs);

	if(enter){

	long sc = regs.orig_rax;

	if(sc >= 0 && sc <= 512){

	syscall_count[sc]++; 

			} // frequency
	
	if(sc_index < MAX_SYSCALLS){

		syscall_stream[sc_index++] = sc;

		} //chronological order of syscall
	
	enter = 0;

	} // entry

	else{

	enter = 1;
	
	} // reentry

		} // data collection

			}// while end 			

		for(int i = 0; i < sc_index - 1; i++){

		int x = syscall_stream[i];
		int y = syscall_stream[i+1];

		// find existing bigram

		int found = 0;
		
		for(int j = 0; j < bigram_count; j++ ){
	
			if(bigrams[j].a == x && bigrams[j].b == y){

				bigrams[j].count++;
				found = 1;
				break;

				}

		} // check already existing bigrams

			if(!found && bigram_count < MAX_BIGRAMS){
			
				bigrams[bigram_count++] = (bigram_t){x,y,1};			

				} // store values
				
			} // logic for bigram

				
	printf("\n=== Syscall Frequency Summary ===\n");
	
	for(int i = 0; i < 512; i++){
	
	if(syscall_count[i] > 0 && syscall_names[i] != NULL) {
	
	printf("%s : %d\n", syscall_names[i], syscall_count[i]);

		}
	
	} // print frequency

	printf("\n=== Syscall Bigrams ===\n");

	for(int i = 0; i < bigram_count; i++){

	int a = bigrams[i].a;
	int b = bigrams[i].b;
	
	if(syscall_names[a] && syscall_names[b]){

		printf("(%s -> %s) : %d\n", syscall_names[a], syscall_names[b], bigrams[i].count);
	
	}	

		} // print bigrams

} // parent process end 

  return 0;

}
