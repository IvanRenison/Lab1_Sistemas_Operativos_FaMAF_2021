#include <stdio.h>
#include <stdbool.h>

#include "command.h"
#include "execute.h"
#include "parser.h"
#include "builtin.h"
#include "prompt.h"


int main(int argc, char *argv[]){
    Parser parser;
    pipeline pipe;
    bool quit = false;

    parser = parser_new(stdin);
    while (!quit) {
        show_prompt();
        pipe = parse_pipeline(parser);
        quit = parser_at_eof(parser); /* Chequeo si hay que salir luego de ejecutar el comando */

        if (pipe != NULL) {
            quit = quit || builtin_is_exit(pipe);
            execute_pipeline(pipe);
            pipeline_destroy(pipe);
        } else if (!quit) {
            printf("Comando no valido\n");
        }
    }
    parser_destroy(parser);
    parser = NULL;
    return 0;
}
