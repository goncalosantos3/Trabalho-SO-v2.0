#include "exePrograms.h"

void executeBasicProgram(char *command[]){

}

// prog1 a b c | prog2 d e f
void executeProgramPipeLine(char *command){
    int nrpipes = 0, i = 0, nresp = 0;
    char *token, *token2;
    const char delimiter[2] = "|";
    const char delimiter2[2] = " ";

    printf("Command: %s\n", command);

    while(command[i] != '\0'){
        if(command[i] == '|'){
            nrpipes++;
        }else if(command[i] == ' '){
            nresp++;
        }
        i++;
    }

    i = 0;
    char *commands[nrpipes+1];

    token = strtok(command, delimiter);
    while (token != NULL) {
        commands[i] = token;    
        printf("Token: %s\n", token);
        token = strtok(NULL, delimiter);
        i++;
    }

    int tam = i;
    i = 0;
    while(i < tam){
        token2 = strtok(commands[i], delimiter2);
        while(token2 != NULL){
            printf("Token2: %s\n", token2);
            token2 = strtok(NULL, delimiter2);
        }
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

