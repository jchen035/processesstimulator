#include <stdio.h>
#include "pidmanager.h"

int main(int argc, char * argv[]){
    int pid1,pid2,pid3,pid4;
    if(allocate_map()){
	pid1 = allocate_pid();
	fprintf(stdout,"First pid %d\n",pid1);
	pid2 = allocate_pid();
	fprintf(stdout,"Second pid %d\n",pid2);
	fprintf(stdout,"Release first pid\n");
	release_pid(pid1);
	pid3 = allocate_pid();
	fprintf(stdout,"Third pid %d\n",pid3);
	pid4 = allocate_pid();
	fprintf(stdout,"Fourth pid %d\n",pid4);
	fprintf(stdout,"Release all pids\n");
	release_pid(pid2);
	release_pid(pid3);
	release_pid(pid4);
	pid1 = allocate_pid();
	fprintf(stdout,"First pid %d\n",pid1);
	release_pid(pid1);
    }
    return 0;
}
