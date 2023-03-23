#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	int pid;

	// Execução de um comando
	if(strcmp(argv[1], "execute") == 0 && strcmp(argv[2], "-u") == 0){
		pid = fork();
		if(pid == 0){ // É o filho
			execvp(argv[3], &argv[4]);
			exit(0);
		}
	}
	return 0;
}
