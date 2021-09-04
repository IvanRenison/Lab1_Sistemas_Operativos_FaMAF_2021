#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "builtin.h"
#include "command.h"
#include "execute.h"


/*int main(void) {

    scommand comando1 = scommand_new();
    scommand_push_back(comando1, strdup("pi"));
    scommand_push_back(comando1, strdup("1000000000"));

    scommand comando2 = scommand_new();
    scommand_push_back(comando2, strdup("ls"));

    scommand comando3 = scommand_new();
    scommand_push_back(comando3, strdup("wc"));
    scommand_push_back(comando3, strdup("-l"));

    pipeline p = pipeline_new();
    pipeline_set_wait(p, true);
    pipeline_push_back(p, comando1);
    pipeline_push_back(p, comando2);
    pipeline_push_back(p, comando3);

    execute_multiple_pipelines(p);

    p = pipeline_destroy(p);

    char** argv = calloc(3, sizeof(char));
    argv[0] = strdup("ctweqewtqd");
    argv[1] = strdup("dafsf");
    argv[2] = NULL;
    execvp(argv[0], argv); 

    pipeline pipe = pipeline_new();
    scommand comando1 = scommand_new();
    scommand comando2 = scommand_new();
    scommand comando3 = scommand_new();
    scommand_push_back(comando1, strdup("ls"));
    scommand_push_back(comando2, strdup("grep"));
    scommand_push_back(comando2, strdup("c"));
    scommand_push_back(comando3, strdup("sort"));
    pipeline_push_back(pipe, comando1);
    pipeline_push_back(pipe, comando2);
    pipeline_push_back(pipe, comando3);
    multiple_pipelines(pipe);
    pipeline_destroy(pipe); 

    return (EXIT_SUCCESS);
}*/
