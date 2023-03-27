#ifndef LLEXEC_H
#define LLEXEC_H
#include <stdio.h>
#include "exec.h"   

// Lista ligada de structs que representam execuções de comandos
typedef struct listaLigadaExe{
    Exec elem;
    struct listaLigadaExe *prox;
}*LLExe;

LLExe initLista();

LLExe insereElem(Exec elem, LLExe lista);

void printaListaExe(LLExe lista);
#endif