#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "exePrograms.h"

/*
*	Execução de programas do utilizador:
*		- O cliente tem que ser capaz de executar comandos atráves da opção "execute -u"
*		- O cliente tem que informar o servidor sobre:
*			1. O PID do processo a executar o programa
*			2. O nome do programa a executar
*			3. O timestamp exatamente antes da execução do programa
*		- O cliente deve notificar o utilizador, via standard output, do PID do programa que será executado.
*		- Após notificar o utilizador e servidor, o cliente deve executar o programa.
*		- Após a terminação de um programa, o cliente deve informar o servidor do mesmo, 
*		  enviando a seguinte informação:
*			1. O PID do processo que terminou
*			2. O timestamp atual, que representa o tempo imediatamente após o término da execução do programa
*		- O cliente deve também notificar o utilizador, via standard output, do tempo de execução 
*		  (em milisegundos) utilizado pelo programa.
*/

// Diferentes invocações válidas do tracer

/*
* ./tracer execute -u "prog-a arg-1 (...) arg-n" (execução de um comando básico)
* ./tracer execute -p "prog-a arg-1 (...) arg-n | prog-b arg-1 (...) arg-n (execução de uma pipeline de comandos)
* ./tracer status (status dos programas em execução)
* ./tracer stats-time PID-1 PID-2 (...) PID-N (tempo total de programas terminados)
* ./tracer stats-command prog-a PID-1 PID-2 (...) PID-N (número de vezes que um programa foi executado)
* ./tracer stats-uniq PID-1 PID-2 (...) PID-N (lista de programas únicos executados)
*/


int main(int argc, char *argv[]){
	int pid, tam;
	char info[100], nome_fifo[100];

	// Abertura do FIFO de escrita
	// Todos os clientes enviam os seus pedidos ao servidor por este FIFO
	int fout = open("/tmp/clients_to_server", O_WRONLY);
	if(fout == -1){
		perror("Erro ao abrir o FIFO para escrita e leitura.");
		exit(-1);
	}

	// Criação do FIFO de leitura
	pid = getpid();
	sprintf(nome_fifo, "/tmp/fifo_%d", pid);
	int p = mkfifo(nome_fifo, 0660);
	if(p==-1){
        if(errno != EEXIST){// Quando o erro não é o erro de o fifo já existir
            printf("Erro ao construir fifo\n");
            exit(-1);
        }
    }

	if(argc == 1 || (strcmp(argv[1], "execute") != 0 && strcmp(argv[1],"status") != 0)){
		sprintf(info, "Invocação inválida do tracer!\n");
		write(1, info, strlen(info) * sizeof(char));
	}else if(strcmp(argv[1], "execute") == 0 && strcmp(argv[2], "-u") == 0){ // Execução de um comando básico
		executeBasicProgram(&argv[3], argc, fout);
	}else if(strcmp(argv[1], "execute") == 0 && strcmp(argv[2], "-p") == 0){ // Execução de uma pipeline de programas
		printf("Pipeline de programas!\n");
		executeProgramPipeLine(argv[3]);
	}else if(strcmp(argv[1], "status") == 0){ // Status
		sprintf(info, "status"); 
		write(fout, info, strlen(info) * sizeof(char));
		
		// Manda o nome do FIFO para o servidor enviar as respostas
		tam = strlen(nome_fifo);
		write(fout, &tam, sizeof(int));
		write(fout, nome_fifo, strlen(nome_fifo) * sizeof(char));
		
		// Abertura do FIFO de leitura
		int fin = open(nome_fifo, O_RDONLY);
		if(fin == -1){
			perror("Erro ao abrir o FIFO para escrita e leitura.");
			exit(-1);
		}

		// Número de programas
		read(fin, &tam, sizeof(int));
		int nr = tam;

		if(nr == 0){
			read(fin, &tam, sizeof(int));
			read(fin, info, tam * sizeof(char));
			write(1, info, tam * sizeof(char));
		}

		for(int i=0; i<nr; i++){
			read(fin, &tam, sizeof(int));
			read(fin, info, tam * sizeof(char));
			write(1, info, tam * sizeof(char));
		}
	}
	return 0;
}
