#ifndef EXEPROGRAMS_H
#define EXEPROGRAMS_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void executeBasicProgram(char *command[], int argc, int fout);

void executeProgramPipeLine(char *command);

#endif