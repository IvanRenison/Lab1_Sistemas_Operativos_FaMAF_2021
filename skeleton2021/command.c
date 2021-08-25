#include <assert.h>
#include <stdio.h>
#include <glib.h>

#include "command.h"


/********** COMANDO SIMPLE **********/

/* Estructura correspondiente a un comando simple.
 * Es una 3-upla del tipo ([char*], char* , char*).
 */

struct scommand_s {
    GList * args;  // GSList
    char * redir_in;
    char * redir_out;
};


scommand scommand_new(void){
    scommand result = malloc(sizeof(struct scommand_s));
    result->args = NULL;
    result->redir_in = NULL;
    result->redir_out = NULL;

    assert(result != NULL &&
        scommand_is_empty(result) &&
        scommand_get_redir_in(result) == NULL &&
        scommand_get_redir_out(result) == NULL
    );

    return result;
}

scommand scommand_destroy(scommand self){
    printf("\nscommand\n");
    assert(self != NULL);

    if(self->args != NULL) {
        printf("\nIF\n");
        g_list_free(self->args);
    }

    free(self->redir_in); self->redir_in = NULL;
    free(self->redir_out); self->redir_out = NULL;
    free(self); self = NULL;

    assert(self == NULL);

    return self;
}

void scommand_push_back(scommand self, char * argument){
    assert(self != NULL && argument != NULL);
}

void scommand_pop_front(scommand self){
}

void scommand_set_redir_in(scommand self, char * filename){
}

void scommand_set_redir_out(scommand self, char * filename){
}

bool scommand_is_empty(const scommand self){
    return true;
}

unsigned int scommand_length(const scommand self){
    return 0;
}

char * scommand_front(const scommand self){
    return NULL;
}

char * scommand_get_redir_in(const scommand self){
    return NULL;
}

char * scommand_get_redir_out(const scommand self){
    return NULL;
}

char * scommand_to_string(const scommand self){
    return NULL;
}





/********** COMANDO PIPELINE **********/

/* Estructura correspondiente a un comando pipeline.
 * Es un 2-upla del tipo ([scommand], bool)
 */

struct pipeline_s {
    GSList *scmds;
    bool wait;
};



pipeline pipeline_new(void){
    return NULL;
}

pipeline pipeline_destroy(pipeline self){
    return NULL;
}

void pipeline_push_back(pipeline self, scommand sc){
}

void pipeline_pop_front(pipeline self){
}

void pipeline_set_wait(pipeline self, const bool w){
}

bool pipeline_is_empty(const pipeline self){
    return true;
}

unsigned int pipeline_length(const pipeline self){
    return 0;
}

scommand pipeline_front(const pipeline self){
    return NULL;
}

bool pipeline_get_wait(const pipeline self){
    return true;
}

char * pipeline_to_string(const pipeline self){
    return NULL;
}


