#ifndef LLEXEC_H
#define LLEXEC_H
#include <stdio.h>
#include <string.h>
#include "exec.h"   

// Lista ligada de structs que representam execuções de comandos
typedef struct listaLigadaExe{
    Exec elem;
    struct listaLigadaExe *prox;
}*LLExe;

LLExe initLista();

void insereElem(Exec exec, LLExe* lista);

void printaListaExe(LLExe* lista);

void execStatus(LLExe* lista, char *nomeFIFO);
#endif