#include "exePrograms.h"

// Exemplo: ls | grep myFile | wc -l
void executeBasicProgram(char *command[], int argc, int fout){
    struct timeval antes, depois;
    int n = 0, pid_exec;
    char info[100];

    int pipefd[2];
    pipe(pipefd);

    if((pid_exec = fork()) == 0){ // Filho vai executar o comando
        close(pipefd[1]); // O filho só vai ler deste pipe
        read(pipefd[0], &n, sizeof(int));
        read(pipefd[0], info, n*sizeof(char));
        close(pipefd[0]);

        if(strcmp(info, "continua") != 0){
            perror("Não pude começar a execução do comando prentendido");
			exit(-1);
        }

        int res = execvp(command[0], command);
		if(res == -1){
			perror("Houve algum erro com a execução do comando pretendido.");
			exit(-1);
		}
    }
    close(pipefd[0]); // O pai só vai escrever para este pipe

	// -> Antes da execução
	// Avisa o servidor que é uma execução de um comando
	sprintf(info, "execan");
	write(fout, info, strlen(info) * sizeof(char));
	// Escreve o PID do processo a executar o programa
	write(fout, &pid_exec, sizeof(int));
	// Nome do programa a executar
	n = strlen(command[0]);
	write(fout, &n, sizeof(int));
	write(fout, command[0], n * sizeof(char));
	// Timestamp
	int r = gettimeofday(&antes, NULL);
    long ts;
	if(r == 0) {
    	ts = antes.tv_sec * 1000000 + antes.tv_usec;
		write(fout, &ts, sizeof(long));
    }else{
		perror("Erro no timestamp.");
		exit(-1);
	}
	// Manda para o utilizador o PID (stdout)
	sprintf(info, "Running PID: %d\n", pid_exec);
	write(1, info, strlen(info) * sizeof(char));
	// <-
    
    // Avisa o filho para começar a executar
    sprintf(info, "continua");
    n = strlen(info);
    write(pipefd[1], &n, sizeof(int));
    write(pipefd[1], info, n*sizeof(char));

	// Espera que o filho acabe de terminar a execução do programa
	wait(NULL);
	// -> Depois da execução
	// Inicia o processo pós-execução
	sprintf(info, "execde");
	write(fout, info, strlen(info) * sizeof(char));
	// Escreve o PID do processo que terminou a execução
	write(fout, &pid_exec, sizeof(int));
	// Timestamp
	r = gettimeofday(&depois, NULL);
	if(r == 0) {
    	ts = depois.tv_sec * 1000000 + depois.tv_usec;
		write(fout, &ts, sizeof(long));
    }else{
		perror("Erro no timestamp.");
		exit(-1);
	}
	// Tempo de execução (para o stdout):
	float diff = (depois.tv_usec - antes.tv_usec) / 1000;
	sprintf(info, "Ended in: %.0fms\n", diff);
	write(1,info,strlen(info)*sizeof(char));
	// <-
}

// prog1 a b c | prog2 d e f
void executeProgramPipeLine(char *command){
    int nrpipes = 0, i = 0;
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
        printf("Token: %s\n", token);
        token = strtok(NULL, delimiter);
        i++;
    }

    i=0;
    while(i<nrpipes+1){
        printf("%d: %s\n", i, commands[i]);
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
            int n = 0, j = 0;

            while(commands[i][j] != '\0'){
                if(commands[i][j] == ' '){
                    n++;
                }
                j++;
            }
            
            char *myCommand[n];

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