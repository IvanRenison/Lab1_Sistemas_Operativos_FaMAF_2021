#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "command.h"

int main(void) {

/*     scommand command = scommand_new();
    scommand_push_back(command, strdup("cd"));
    scommand_push_back(command, strdup(".."));

    char* primero = scommand_front(command);
    printf("%s\n", primero);

    command = scommand_destroy(command); */

    //pipeline pipe = pipeline_new();
/*     scommand command = scommand_new();
    scommand_push_back(command, strdup("cd"));
    scommand_push_back(command, strdup("~/Desktop")); */

    char* current = malloc(1000);
    current = getcwd(current, 1000);
    printf("%s\n", current);


    printf("CHDIR: %d\n", chdir("~/Desktop"));

    perror("~/Desktop");

/*     char* c = scommand_to_string(command);
    printf("%s\n", c);
    free(c); */


//    builtin_scommand_exec(command);

    current = getcwd(current, 1000);
    printf("%s\n", current);
    free(current);

//    command = scommand_destroy(command);

    return(EXIT_SUCCESS);
}
