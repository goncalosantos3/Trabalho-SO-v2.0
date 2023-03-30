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

// Remove da lista ligada um programa que já terminou a sua execução
void removeElem(int pid, LLExe* lista){
    Exec aux;
    int removido = 0;

    while((*lista) != NULL && removido == 0){
        if((*lista)->elem->pid == pid){
            aux = (*lista)->elem;
            (*lista) = (*lista)->prox;
            free(aux); // Liberta a memória do elemento removido
            removido = 1;
        }else{
            lista = &(*lista)->prox;
        }
    }   
}

void printaListaExe(LLExe* lista){

    while((*lista) != NULL){
        printaExec((*lista)->elem);
        lista = &(*lista)->prox;
    }
}

// Devolve o número de elementos na lista ligada
int tamLista(LLExe *lista){
    int tam = 0;

    while((*lista) != NULL){
        tam++;
        lista = &(*lista)->prox;
    }

    return tam;
}

void execStatus(LLExe* lista, char *nomeFIFO){
    char info[100];
    int fout = open(nomeFIFO, O_WRONLY);
    if(fout == -1){
        perror("Houve um problema ao abrir o FIFO!");
    }

    int tam = tamLista(lista);
    // Manda o número de programas na lista para o cliente
    write(fout, &tam, sizeof(int));

    if(tam == 0){
        sprintf(info, "No programs to show!\n");
        tam = strlen(info);
        write(fout, &tam, sizeof(int));
        write(fout, info, tam * sizeof(char));
    }

    while((*lista) != NULL){
        sprintf(info, "%d %s %lims\n", (*lista)->elem->pid, (*lista)->elem->nome, (*lista)->elem->tempo);
        tam = strlen(info);
        write(fout, &tam, sizeof(int));
        write(fout, info, strlen(info) * sizeof(char));
        lista = &(*lista)->prox;
    }
}
