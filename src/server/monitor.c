#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Execucao { 
  int pid;          
  char *nome;
  int tempo;     
}*Exec;

int main(int argc, char *argv[]){
	// Construção do FIFO
	int p = mkfifo("../fifo",0660);
    if(p==-1){
        if(errno != EEXIST){// Quando o erro não é o erro de o fifo já existir
            printf("Erro ao construir fifo\n");
            return 1;
        }
    }

	// Abertura do FIFO
	int f = open("fifo", O_RDWR);
	if(f == -1){
		perror("Erro ao abrir FIFO para ler e escrever");
		exit(-1);
	}



	return 0;
}
