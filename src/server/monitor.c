#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "exec.h"
#include "llExec.h"


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
	int f = open("../fifo", O_RDWR);
	if(f == -1){
		perror("Erro ao abrir FIFO para ler e escrever");
		exit(-1);
	}

    char *execs[3] = {"exe1","exe2","exe3"};
    LLExe lista = initLista();
    for(int i=0; i < 3; i++){
        Exec exec = buildExec(i,i,execs[i]);
        lista = insereElem(exec, lista);
    }
    printaListaExe(lista);
    printf("Outra vez!\n");
    printaListaExe(lista);
	return 0;
}
