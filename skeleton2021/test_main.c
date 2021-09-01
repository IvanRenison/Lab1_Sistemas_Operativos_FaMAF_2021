#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "command.h"
#include "execute.h"

int main(void) {

    scommand comando = scommand_new();

    scommand_push_back(comando, strdup("wc"));
    scommand_push_back(comando, strdup("-l"));
    scommand_set_redir_in(comando, strdup("a.txt"));
    scommand_set_redir_out(comando, strdup("out.txt"));
    
    scommand_exec(comando);
    scommand_destroy(comando);

    return (EXIT_SUCCESS);
}
