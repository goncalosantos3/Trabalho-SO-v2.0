#ifndef EXEC_H
#define EXEC_H
#include <stdlib.h>
#include <stdio.h>

// Struct que vai guardar a informação sobre todos os pedidos de execução
typedef struct execucao { 
  int pid;          
  char *nome;
  long tempo;     
}*Exec;

Exec buildExec(int pid, long tempo, char *nome);

void printaExec(Exec exe);
#endif
