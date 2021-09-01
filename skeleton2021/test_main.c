#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "command.h"
#include "execute.h"

int main(void) {

    scommand comando = scommand_new();

    scommand_push_back(comando, strdup("gcc"));
    scommand_push_back(comando, strdup("*.c"));

    scommand_exec(comando);
    scommand_destroy(comando);

    return (EXIT_SUCCESS);
}
