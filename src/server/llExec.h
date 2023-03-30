#ifndef LLEXEC_H
#define LLEXEC_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "exec.h"   

// Lista ligada de structs que representam execuções de comandos
typedef struct listaLigadaExe{
    Exec elem;
    struct listaLigadaExe *prox;
}*LLExe;

LLExe initLista();

void insereElem(Exec exec, LLExe* lista);

void removeElem(int pid, LLExe* lista);

void printaListaExe(LLExe* lista);

void execStatus(LLExe* lista, char *nomeFIFO);
#endif