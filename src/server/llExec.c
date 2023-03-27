#include <stdlib.h>
#include <string.h>
#include "llExec.h"

// Inicia a lista ligada de struct execucao
// Esta função não faz nada basicamente porque a memória é alocada ao inserir um elemento
LLExe initLista(){
    LLExe lista = NULL;
    return lista;
}

// Esta função insere o elemento à cabeça da lista ligada
void insereElem(Exec exec, LLExe* lista){
    LLExe elem = malloc(sizeof(struct listaLigadaExe));
    elem->elem = exec;
    elem->prox = (*lista);
    (*lista) = elem;
}

void printaListaExe(LLExe* lista){

    while((*lista) != NULL){
        printaExec((*lista)->elem);
        lista = &(*lista)->prox;
    }
}

void execStatus(LLExe* lista, int fd){
    char info[100];

    while((*lista) != NULL){
        sprintf(info, "%d %s %dms\n", (*lista)->elem->pid, (*lista)->elem->nome, (*lista)->elem->tempo);
        write(1, info, strlen(info)*sizeof(char));
    }
}
