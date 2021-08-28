#include "command.h"
#include <glib.h>
#include <stdio.h>
#include <string.h>


int main(void) {



//    pipeline comandos = pipeline_new();

    scommand comando = scommand_new();
    scommand_push_back(comando, strdup("gcc"));
    scommand_push_back(comando, strdup("-Wall"));

    scommand_pop_front(comando);
    scommand_pop_front(comando);

    comando = scommand_destroy(comando);


/*     pipeline_push_back(comandos, comando);

    comando = scommand_new();
    scommand_push_back(comando, strdup("gcc2"));
    scommand_push_back(comando, strdup("-Wall2"));

    pipeline_push_back(comandos, comando);

    pipeline_set_wait(comandos, 1);

    char* cs = pipeline_to_string(comandos);
    printf("%s", cs);
    free(cs);
    comandos = pipeline_destroy(comandos); */

    return(EXIT_SUCCESS);
}





