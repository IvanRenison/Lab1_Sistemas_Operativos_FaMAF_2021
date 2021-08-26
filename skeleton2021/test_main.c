#include "command.h"
#include <glib.h>
#include <stdio.h>


int main(void) {
    char * arg = "exec";
    char * redir_in = "text.in";
    char * redir_out = "text.out";
    char * test1 = NULL;
    char * test2 = NULL;

    scommand comando = scommand_new();
    scommand_push_back(comando, arg);
    printf("%d\n", scommand_length(comando));
    char * front = scommand_front(comando);
    printf("%s\n", front);
    front = front;
    scommand_pop_front(comando);
    scommand_set_redir_in(comando, redir_in);
    scommand_set_redir_out(comando, redir_out);
    test1 = scommand_get_redir_in(comando);
    test2 = scommand_get_redir_out(comando);
    printf("%s\n", test1);
    printf("%s\n", test2);
    printf("%d\n", scommand_length(comando));
    comando = scommand_destroy(comando);

    return(1);
}





