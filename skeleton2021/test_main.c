#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "command.h"

int main(void) {

    /*scommand command = scommand_new();
    scommand_push_back(command, strdup("cd"));
    scommand_push_back(command, strdup("..//.vscode"));

    char* current = malloc(1000);
    current = getcwd(current, 1000);
    printf("%s\n", current);
    free(current);

    pipeline p = pipeline_new();
    pipeline_push_back(p, command);

    builtin_single_pipeline_exec(p);
    
    current = malloc(1000);
    current = getcwd(current, 1000);
    printf("%s\n", current);
    free(current);


    p = pipeline_destroy(p);*/

    //Prueba de cd 
    scommand comando = scommand_new();
    char * cm = strdup("cd");
    char * dir = strdup("''~''/a");
    scommand_push_back(comando, cm);
    scommand_push_back(comando, dir);

    char* current = malloc(1000);
    current = getcwd(current, 1000);
    printf("%s\n", current);
    
    builtin_scommand_exec(comando);

    current = getcwd(current, 1000);
    printf("%s\n", current);

    free(current);
    scommand_destroy(comando);

    return(EXIT_SUCCESS);
}
