#include <stdio.h>
#include <unistd.h>

int main(){

printf("Target running. PID=%d\n", getpid());
sleep(3);
printf("Target exiting.\n");

return 0;
}
