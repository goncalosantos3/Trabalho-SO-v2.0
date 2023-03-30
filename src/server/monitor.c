#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "exec.h"
#include "llExec.h"


int main(int argc, char *argv[]){
    long tempo;
    int n, pid, tam;
    char info[100], nome[100];

	// Construção do FIFO
    // Este FIFO vai ser o FIFO utilizado por todos os clientes para fazer pedidos ao servidor
	int p = mkfifo("/tmp/clients_to_server",0660);
    if(p==-1){
        if(errno != EEXIST){// Quando o erro não é o erro de o fifo já existir
            printf("Erro ao construir fifo\n");
            return 1;
        }
    }

	// Abertura do FIFO
    // O servidor apenas lê deste FIFO
	int fin = open("/tmp/clients_to_server", O_RDONLY);
	if(fin == -1){
		perror("Erro ao abrir FIFO para ler e escrever");
		exit(-1);
	}

    int fout = open("/tmp/clients_to_server", O_WRONLY);
    if(fout == -1) {
        printf("%s\n", strerror(errno));
        exit(-1);
    }

    // Lista que vai ter as execuções atuais
    LLExe l = initLista();
    LLExe *lista = &l;
    while((n=read(fin, info, 6 * sizeof(char))) > 0){
        info[6] = '\0';
        printf("Info: %s\n", info);
        if(strcmp(info,"status") == 0){ // Status de todos os programas
            printf("Status!\n");
            // Recebe o nome do FIFO de escrita
            read(fin, &tam, sizeof(int));
            read(fin, info, tam * sizeof(char));

            // Um processo filho trata os pedidos "status"
            if(fork() == 0){
                execStatus(lista, info); // Envia a informação para o cliente
                exit(0);
            }
        }else if(strcmp(info, "execan") == 0){ // Pré execução de um programa
            printf("Execute antes!\n");
            // PID
            read(fin, &pid, sizeof(int));
            printf("PID: %d\n", pid);
            // Nome
            read(fin, &tam, sizeof(int));
            read(fin, &nome, tam * sizeof(char));
            printf("Nome: %s\n", nome);
            // TimeStamp
            read(fin, &tempo, sizeof(long));
            printf("TimeStamp: %li\n", tempo);

            Exec exe = buildExec(pid, tempo, nome);
            insereElem(exe, lista);
        }else if(strcmp(info, "execde") == 0){ // Pós execução de um programa
            printf("Execute depois!\n");

            // PID
            read(fin, &pid, sizeof(int));
            printf("PID: %d\n", pid);
            // TimeStamp
            read(fin, &tempo, sizeof(long));
            printf("TimeStamp: %li\n", tempo);

            // Remove o programa da lista ligada que terminou a sua execução
            removeElem(pid, lista);
        }
    }

	return 0;
}
