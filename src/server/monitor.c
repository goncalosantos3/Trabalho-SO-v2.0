#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "exec.h"
#include "llExec.h"


int main(int argc, char *argv[]){
    int n, pid, tam;
    char info[100], nome[100], tempo[100];

	// Construção do FIFO
	int p = mkfifo("../fifo",0660);
    if(p==-1){
        if(errno != EEXIST){// Quando o erro não é o erro de o fifo já existir
            printf("Erro ao construir fifo\n");
            return 1;
        }
    }

	// Abertura do FIFO
	int fifo = open("../fifo", O_RDWR);
	if(fifo == -1){
		perror("Erro ao abrir FIFO para ler e escrever");
		exit(-1);
	}

    // Lista que vai ter as execuções atuais
    LLExe l = initLista();
    LLExe *lista = &l;
    while((n=read(fifo,info,sizeof(info))) > 0){
        if(strcmp(info,"status") == 0){ // Status de todos os programas
            printf("Status!");
            execStatus(lista, fifo); // Envia a informação para o cliente
        }else if(strcmp(info, "execute") == 0){ // Execução de um programa
            printf("Execute!\n");
            // PID
            read(fifo, &pid, sizeof(int));
            // Nome
            read(fifo, &tam, sizeof(int));
            read(fifo, nome, tam * sizeof(char));
            // TimeStamp
            read(fifo, &tam, sizeof(int));
            read(fifo, tempo, tam * sizeof(char));

            // buildExec(pid, tempo, nome);
            printf("%d %s %s\n", pid, nome, tempo);
        } 
        printf("Olá\n");
    }

	return 0;
}
