#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "execute.h"
#include "command.h"

int main(void) {

    scommand comando = scommand_new();
    
    scommand_push_back(comando, strdup("gcc"));

    scommand_exec(comando);
    //scommand_push_back(comando, strdup("*.c"));
    scommand_destroy(comando);

    return(EXIT_SUCCESS);
}
