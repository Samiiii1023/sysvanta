#include <stdio.h>
#include <unistd.h>

int main(){

printf("Target program running. PID=%d\n", getpid());
sleep(1);
printf("Target program exiting.\n");
return 0;

}
