#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>

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

int main(int argc, char *argv[]){
	struct timeval antes, depois;
	long ts;
	int pid, pid_exec, exit_status, r, tam;
	char info[100], nome_fifo[100];

	// Abertura do FIFO de escrita
	// Todos os clientes enviam os seus pedidos ao servidor por este FIFO
	int fout = open("clients_to_server", O_WRONLY);
	if(fout == -1){
		perror("Erro ao abrir o FIFO para escrita e leitura.");
		exit(-1);
	}

	// Criação do FIFO de leitura
	pid = getpid();
	sprintf(nome_fifo, "fifo_%d", pid);
	int p = mkfifo(nome_fifo, 0660);
	if(p==-1){
        if(errno != EEXIST){// Quando o erro não é o erro de o fifo já existir
            printf("Erro ao construir fifo\n");
            exit(-1);
        }
    }

	// Abertura do FIFO de leitura
	int fin = open(nome_fifo, O_RDONLY);
	if(fin == -1){
		perror("Erro ao abrir o FIFO para escrita e leitura.");
		exit(-1);
	}


	// Execução de um comando
	if(strcmp(argv[1], "execute") == 0 && strcmp(argv[2], "-u") == 0){
		pid_exec = fork();
		if(pid_exec == 0){ // É o filho
			int res = execvp(argv[3], &argv[3]);
			if(res == -1){
				perror("Houve algum erro com a execução do comando pretendido.");
				exit(-1);
			}
		}
		
		// -> Antes da execução
		// Avisa o servidor que é uma execução de um comando
		sprintf(info, "exec");
		write(fout, info, strlen(info) * sizeof(char));
		// Escreve o PID do processo a executar o programa
		write(fout, &pid_exec, sizeof(int));
		// Nome do programa a executar
		tam = strlen(argv[3]);
		write(fout, &tam, sizeof(int));
		write(fout, argv[3], strlen(argv[3]) * sizeof(char));
		// Timestamp
		r = gettimeofday(&antes, NULL);
		if(r == 0) {
        	ts = antes.tv_sec * 1000000 + antes.tv_usec;
			write(fout, &ts, sizeof(long));
    	}else{
			perror("Erro no timestamp.");
			exit(-1);
		}
		// Manda para o utilizador o PID (stdout)
		sprintf(info, "Running PID: %d\n", pid_exec);
		write(1, info, strlen(info) * sizeof(char));
		// <-

		// Espera que o filho acabe de terminar a execução do programa
		wait(&exit_status);

		// -> Depois da execução
		// Escreve o PID do processo que terminou a execução
		sprintf(info, "%d", pid_exec);
		write(fout, info, strlen(info) * sizeof(char));
		// Timestamp
		r = gettimeofday(&depois, NULL);
		if(r == 0) {
        	ts = depois.tv_sec + depois.tv_usec;
			write(fout, info, strlen(info) * sizeof(char));
    	}else{
			perror("Erro no timestamp.");
			exit(-1);
		}
		// Tempo de execução:
		float diff = (depois.tv_sec - antes.tv_sec) * 1000;
		sprintf(info, "Ended in: %.0fms\n", diff);
		write(1,info,strlen(info)*sizeof(char));
		// <-
	}else if(strcmp(argv[1], "status") == 0){ // Status
		sprintf(info, "stat"); 
		write(fout, info, strlen(info) * sizeof(char));
		
		// Manda o nome do FIFO para o servidor enviar as respostas
		tam = strlen(nome_fifo);
		write(fout, &tam, sizeof(int));
		write(fout, nome_fifo, strlen(nome_fifo) * sizeof(char));
		
		// Número de programas
		read(fin, &tam, sizeof(int));
		int nr = tam;
		for(int i=0; i<nr; i++){
			read(fin, &tam, sizeof(int));
			read(fin, info, tam * sizeof(char));
			write(1, info, tam * sizeof(char));
		}
	}
	return 0;
}
