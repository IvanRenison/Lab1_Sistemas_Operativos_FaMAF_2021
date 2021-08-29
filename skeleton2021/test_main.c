#include "command.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(void) {

    FILE* test = fopen("test.txt", "r");
    Parser p = parser_new(test);

    pipeline comando = parse_pipeline(p);
    char* xs = pipeline_to_string(comando);
    printf("%s\n", xs);

    free(xs);
    pipeline_destroy(comando);
    parser_destroy(p);


    fclose(test);
    return(EXIT_SUCCESS);
}





