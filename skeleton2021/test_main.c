#include "command.h"
#include <glib.h>
#include <stdio.h>


int main(void) {
    char * arg = "exec";
    char * redir_in = "text.in";
    char * redir_out = "text.out";

    scommand comando = scommand_new();
    scommand_push_back(comando, arg);
    printf("%d", scommand_length(comando));
    scommand_pop_front(comando);
    char * front = scommand_front(comando);
    front = front;
    scommand_set_redir_in(comando, redir_in);
    scommand_set_redir_out(comando, redir_out);
    printf("%d", scommand_length(comando));
    comando = scommand_destroy(comando);

    return(1);
}





