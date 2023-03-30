#include "exePrograms.h"

void executeBasicProgram(char *command[]){

}

// prog1 a b c | prog2 d e f
void executeProgramPipeLine(char *command){
    int nrpipes = 0, i = 0, r;
    char *token;
    const char delimiter[2] = "|";

    printf("Command: %s\n", command);

    while(command[i] != '\0'){
        if(command[i] == '|'){
            nrpipes++;
        }
        i++;
    }

    i = 0;
    char *commands[nrpipes+1];

    token = strtok(command, delimiter);
    while (token != NULL) {
        commands[i] = token;
        token = strtok(NULL, delimiter);
        i++;
    }

    // Inicialização dos pipes
    int pipes[nrpipes][2];
    for(i=0; i<nrpipes; i++){
        pipe(pipes[i]);
    }

    // Vamos ter que criar nrpipes+1 filhos
    for(i=0; i<nrpipes+1; i++){
        // Cria o filho
        if(fork() == 0){
            if(i==0){ // Primeiro filho
                // O primeiro filho apenas escreve para o primeiro pipe
                close(pipes[0][0]);
                for(int j=1; j<nrpipes; j++){
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
                dup2(pipes[0][1], 1);
                close(pipes[0][1]);

                
            }else if(i<nrpipes){ // Filhos do meio

            }else{ // Último filho

            }
        }
    }
}

