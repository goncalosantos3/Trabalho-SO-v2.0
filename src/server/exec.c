#include "exec.h"

// Função que constroi uma struct de execução
Exec buildExec(int pid, long tempo, char *nome){
    Exec exe = malloc(sizeof(struct execucao));

    exe->pid = pid;
    exe->tempo = tempo;
    exe->nome = nome;

    return exe;
}

void printaExec(Exec exe){
    printf("PID: %d\nNome: %s\nTempo: %lli\n", exe->pid, exe->nome, exe->tempo);
}