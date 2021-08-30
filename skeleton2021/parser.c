#include <stdio.h> 		/* FILE */
#include <string.h>
#include <stdbool.h>	/* bool */

#include "command.h"	/* pipeline */
#include "parser.h"     /* parser */

typedef struct parser_s * Parser;

// struct _IO_FILE;

// typedef struct _IO_FILE FILE;

// Notar que el TAD Parser toma un FILE * como entrada y como en *NIX,
// todo es un archivo, la entrada estándar también tiene un FILE * asociado.

Parser parser_new(FILE *input){
    assert(input != NULL);
    Parser parse = malloc(sizeof(struct parser_s *));
    parse->strin = NULL;
    parse->input = input;
    parse->iseof = false;
    return parse;
}

Parser parser_destroy(Parser parser){
    assert(parser != NULL);
    free(parser->strin);
    free(parser->input);
    free(parser);
    return NULL;
}

Parser change_iseof(Parser parser, bool type){
    parser->iseof = type;
    return parser
}

// hola | echo "hola" \n hola && echo "hola" ...

pipeline parse_pipeline(Parser parser){
    assert(parser != NULL);
    assert(parser->strin == NULL);  // Revisar
    assert(!parser_at_eof(parser));
    pipeline pipe = malloc(sizeof(pipeline));
    char getme = ' ';
    // Arreglo de caracteres -> NULL cuando termina
    char aux_a [2] = " ";  // String 2 chars: 1st " " 2nd NULL
    while(!(parser->iseof) && getme != '\n'){
        getme = (char)getcf(parser->input);
        if(getme == EOF){
            parser = change_iseof(parser, true);
        }
        // Interpreta como el 1st caracter
        aux_a[0] = getme;  // char -> string
        parser->strin = strcat(parser->strin, aux_a);
    };
    printf("\n%s", parser->strin);
    return pipe;
}

bool parser_at_eof(Parser parser){
    assert(parser != NULL);
    return parser->iseof;
}
