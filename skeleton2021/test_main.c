#include "command.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(void) {

    /*FILE* test = fopen("test.txt", "r");
    Parser p = parser_new(test);

    pipeline comando = parse_pipeline(p);
    char* xs = pipeline_to_string(comando);
    printf("%s\n", xs);

    free(xs);
    pipeline_destroy(comando);
    parser_destroy(p);


    fclose(test);*/

    char * arg1 = strdup("exec");
    char * arg2 = strdup("wc");
    char * redir_in = strdup("file.in");
    scommand comando = scommand_new();
    scommand_push_back(comando, arg1);
    printf("%d\n", scommand_length(comando));
    scommand_set_redir_in(comando, redir_in);
    printf("%s\n", scommand_front(comando));
    scommand_pop_front(comando);
    printf("%d\n", scommand_length(comando));
    scommand_push_back(comando, arg2);
    char * command = scommand_to_string(comando);
    printf("%s\n", command);
    free(command);
    scommand_destroy(comando);


    return(EXIT_SUCCESS);
}





