#include <stdbool.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtin.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "prompt.h"

int main(int argc, char *argv[]){

    // Inicializo exit_from_mybash para que no salga
    exit_from_mybash = false;

    Parser parser;
    pipeline pipe;

    parser = parser_new(stdin);
    while (!exit_from_mybash) {
        // exit_from_mybash es una variable global declarada en builtin.h
        show_prompt();
        pipe = parse_pipeline(parser);

        /* Si se llegó a un final de archivo siginifca que hay que salir después
           de ejecutar el comando */
        exit_from_mybash = parser_at_eof(parser);

        if (pipe != NULL) {
            execute_pipeline(pipe);
            pipeline_destroy(pipe);
        } else if (!exit_from_mybash && pipe != NULL) {
            printf("Comando no valido\n");
        }
    }
    parser_destroy(parser);
    parser = NULL;
    return 0;
} 
