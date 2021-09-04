#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "builtin.h"
#include "command.h"
#include "execute.h"

void multiple_pipelines(pipeline p);
void pipeline2(scommand cmd1, scommand cmd2);
void pipeline2(scommand cmd1, scommand cmd2) {
    int pipefd[2];
    int res_pipe = pipe(pipefd);
    assert(res_pipe == 0); ///
    // pipefd[0] es la punta de lectura y pipefd[1] la de escritura
    int punta_lectura = pipefd[0];
    int punta_escitura = pipefd[1];

    int pid = fork();
    if (pid == -1) {
        // Caso de error

    }
    else if (pid == 0) {
        // El hijo
        // Acá se escribé en la punta de escritura
        close(punta_lectura);
        dup2(punta_escitura, STDOUT_FILENO);
        close(punta_escitura);
        scommand_exec(cmd1);
    }
    else {
        // El padre
        close(punta_escitura);
        int pid2 = fork();
        if (pid2 == -1) {
            // Caso de error

        }
        else if (pid2 == 0) {
            // El hijo
            // Acá se lee de la punta de lectura
            dup2(punta_lectura, STDIN_FILENO);
            close(punta_lectura);
            scommand_exec(cmd2);
        }
        else {
            // El padre
            close(punta_lectura);
            wait(NULL);
            wait(NULL);

        }
    }

}

void multiple_pipelines(pipeline p){
    int numberOfPipes = pipeline_length(p) - 1;

    int status;
    int i = 0;
    pid_t pid;

    int * pipefds = calloc(2*numberOfPipes, sizeof(int));

    for(i = 0; i < (numberOfPipes); i ++){
        if(pipe(pipefds + i*2) < 0){
            perror("Pipe failed");
        }
    }

    int j = 0;
    while(!pipeline_is_empty(p)){
        pid = fork();
        if(pid < 0){
            perror("Fork failed");
            pipeline_destroy(p);
            _exit(-2);
        } else if (pid == 0){
            
            if(pipeline_length(p) > 1){
                if(dup2(pipefds[j + 1], STDOUT_FILENO) < 0){
                    perror("dup2");
                    pipeline_destroy(p);
                }
            }

            if(j != 0){
                if(dup2(pipefds[j - 2], STDIN_FILENO) < 0){
                    perror("dup2");
                    pipeline_destroy(p);            
                }
            }

            for(i = 0; i < 2 * numberOfPipes; i++){
                close(pipefds[i]);
            }

            if(builtin_scommand_is_internal(pipeline_front(p))){
                builtin_scommand_exec(pipeline_front(p));
            } else if (scommand_exec(pipeline_front(p)) == -1) {
                _exit(-2);
            }
        } 
        if(!pipeline_is_empty(p)){
            pipeline_pop_front(p);
        }
        j += 2;
    } 

    for(i = 0; i < 2 * numberOfPipes; i++){
        close(pipefds[i]);
    }

    for(i = 0; i <= numberOfPipes ; i++){
        wait(&status);
    }

    free(pipefds);
}


int main(void) {

    /*scommand comando1 = scommand_new();
    scommand_push_back(comando1, strdup("ls -l"));

    scommand comando2 = scommand_new();
    scommand_push_back(comando2, strdup("wc -l"));
    scommand_push_back(comando2, strdup("c"));

    pipeline2(comando1, comando2);

    comando1 = scommand_destroy(comando1);
    comando2 = scommand_destroy(comando2);*/

    pipeline pipe = pipeline_new();
    scommand comando1 = scommand_new();
    scommand comando2 = scommand_new();
    scommand comando3 = scommand_new();
    scommand_push_back(comando1, strdup("ls"));
    scommand_push_back(comando2, strdup("grep"));
    scommand_push_back(comando2, strdup("c"));
    scommand_push_back(comando3, strdup("sort"));
    pipeline_push_back(pipe, comando1);
    pipeline_push_back(pipe, comando2);
    pipeline_push_back(pipe, comando3);
    multiple_pipelines(pipe);
    pipeline_destroy(pipe);

    return (EXIT_SUCCESS);
}
