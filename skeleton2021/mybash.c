#include <stdbool.h>
#include <stdio.h>

#include "builtin.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "prompt.h"

int main(int argc, char* argv[]) {

    // Inicializo exit_from_mybash para que no salga
    exit_from_mybash = false;

    Parser parser = parser_new(stdin);

    while (!exit_from_mybash) {
        // exit_from_mybash es una variable global declarada en builtin.h
        show_prompt();
        pipeline apipe = parse_pipeline(parser);

        /* Si se llegó a un final de archivo siginifca que hay que salir después
           de ejecutar el comando */
        exit_from_mybash = parser_at_eof(parser);

        if (apipe != NULL) {
            execute_pipeline(apipe);
            apipe = pipeline_destroy(apipe);
        }
    }

    // Antes de salir se imprime un salto de linea
    printf("\n");
    parser = parser_destroy(parser);
    return 0;
}
