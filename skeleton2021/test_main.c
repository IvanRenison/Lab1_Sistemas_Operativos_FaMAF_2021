#include "command.h"
#include <glib.h>
#include <stdio.h>


int main(void) {
    scommand comando = scommand_new();

    char* word = strdup("Hola");
    scommand_push_back(comando, word);
    word = strdup("mundo");
    scommand_push_back(comando, word);
    word = strdup("");
    scommand_push_back(comando, word);

    scommand_pop_front(comando);

    char* comandoLindo = scommand_to_string(comando);
    printf("%s\n", comandoLindo);
    comando = scommand_destroy(comando);
    free(comandoLindo); comandoLindo = NULL;

/*     char * arg1 = strdup("exec");
    char * arg2 = strdup("wc");
    char * redir_in = strdup("text.in");
    char * redir_out = strdup("text.out");
    char * test1 = NULL;
    char * test2 = NULL;
    char * command = NULL;

    scommand comando = scommand_new();
    scommand_push_back(comando, arg1);
    scommand_push_back(comando, arg2);
    scommand_set_redir_in(comando, redir_in);
    scommand_set_redir_out(comando, redir_out);
    command = scommand_to_string(comando);
    printf("%s\n", command);
    printf("%d\n", scommand_length(comando));
    char * front = scommand_front(comando);
    printf("%s\n", front);
    front = front;
    scommand_pop_front(comando);
    test1 = scommand_get_redir_in(comando);
    test2 = scommand_get_redir_out(comando);
    printf("%s\n", test1);
    printf("%s\n", test2);
    printf("%d\n", scommand_length(comando));
    comando = scommand_destroy(comando);
    free(command);
    free(arg1);
    free(arg2); */


    return(EXIT_SUCCESS);
}





