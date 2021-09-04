#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "command.h"
#include "execute.h"

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
            wait(NULL);
            wait(NULL);

        }
    }

}


int main(void) {

    scommand comando1 = scommand_new();
    scommand_push_back(comando1, strdup("ls"));

    scommand comando2 = scommand_new();
    scommand_push_back(comando2, strdup("grep"));
    scommand_push_back(comando2, strdup("c"));
    scommand_set_redir_out(comando2, strdup("text.out"));

    pipeline2(comando1, comando2);

    comando1 = scommand_destroy(comando1);
    comando2 = scommand_destroy(comando2);

    return (EXIT_SUCCESS);
}
