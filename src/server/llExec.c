#include <stdlib.h>
#include "llExec.h"

// Inicia a lista ligada de struct execucao
// Esta função não faz nada basicamente porque a memória é alocada ao inserir um elemento
LLExe initLista(){
    LLExe lista = NULL;
    return lista;
}

// Esta função insere o elemento à cabeça da lista ligada
LLExe insereElem(Exec exec, LLExe lista){
    LLExe elem = malloc(sizeof(struct listaLigadaExe));
    elem->elem = exec;
    elem->prox = lista;
    return elem;
}

void printaListaExe(LLExe lista){
    LLExe *aux = &lista;

    while((*aux) != NULL){
        printaExec((*aux)->elem);
        (*aux) = (*aux)->prox;
    }
}
