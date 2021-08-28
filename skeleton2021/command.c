#include <assert.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "command.h"


/********** COMANDO SIMPLE **********/

/* Estructura correspondiente a un comando simple.
 * Es una 3-upla del tipo ([char*], char* , char*).
 */

struct scommand_s {
    GSList * args;
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

scommand scommand_destroy(scommand self) {
    assert(self != NULL);

    g_slist_free_full(self->args, free); self->args = NULL;

    free(self->redir_in); self->redir_in = NULL;
    free(self->redir_out); self->redir_out = NULL;

    free(self); self = NULL;

    assert(self == NULL);

    return self;
}

void scommand_push_back(scommand self, char * argument){
    assert(self != NULL && argument != NULL);

	self->args = g_slist_append(self->args, argument);

	assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self){
	assert(self != NULL && !scommand_is_empty(self));

    free(g_slist_nth_data(self->args, 0));  // Libera el primer elemento
    GSList* temp = self->args;              // Obtiene el puntero a la cabeza de la lista
    self->args = g_slist_next(self->args);  // Avansa la lista
    free(temp);                             // Libera la cabeza de la lista
}

void scommand_set_redir_in(scommand self, char * filename){
	assert(self != NULL);

	self->redir_in = filename;
}

void scommand_set_redir_out(scommand self, char * filename){
	assert(self != NULL);

	self->redir_out = filename;
}

bool scommand_is_empty(const scommand self){
	assert(self != NULL);

    return (self->args == NULL);
}

unsigned int scommand_length(const scommand self){
	assert(self != NULL);

	unsigned int length = g_slist_length(self->args);
    // g_slist_length devuelve un guint que es lo mismo que un unsigned int

	assert((length == 0) == scommand_is_empty(self));
    return length;
}

char * scommand_front(const scommand self){
    assert(self != NULL && !scommand_is_empty(self));

    char * result = g_slist_nth_data(self->args, 0);

    assert(result != NULL);
    return result;
}

char * scommand_get_redir_in(const scommand self){
	assert(self != NULL);

    return (self->redir_in);
}

char * scommand_get_redir_out(const scommand self){
	assert(self != NULL);
    return (self->redir_out);
}


/* Concatena 2 string reallocando el primero para agregar el espacio necesario.
 * xs tiene que ser un string dinamico, pero ys puede ser estático.
 * Notar que es distinto de strmerge, ya que strmerge
 * Uso: xs = str_concat(xs, ys);
 *      xs = str_concat(xs, "string");
 * Requires: xs != NULL && ys != NULL
 */
static char * str_concat(char * xs, const char * ys) {
    assert(xs != NULL && ys != NULL);

    size_t xs_len = strlen(xs);
    size_t ys_len = strlen(ys);

	xs = realloc(xs, xs_len + ys_len + sizeof('\0'));

	xs = strcat(xs, ys);

    return(xs);
}


char * scommand_to_string(const scommand self){
    GSList* xs = self->args;
    char* result = strdup("");

    if(xs != NULL) {
        result = str_concat(result, xs->data);
        while(xs->next != NULL) {
            xs = g_slist_next(xs);
            result = str_concat(result, " ");
            result = str_concat(result, xs->data);
        }
    }

    if(self->redir_out != NULL) {
        result = str_concat(result, " > ");
        result = str_concat(result, self->redir_out);
    }

    if(self->redir_in != NULL) {
        result = str_concat(result, " < ");
        result = str_concat(result, self->redir_in);
    }

    return(result);
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
    pipeline result = malloc(sizeof(struct pipeline_s));
    result->scmds = NULL;
    result->wait = true;

    assert(result != NULL
        && pipeline_is_empty(result)
        && pipeline_get_wait(result)
    );
    return result;
}

/* g_slist_free_full necesita una función que devuelva void
 * para liberar una lista de scommand se puede usar esta función
 */ // Se debe poder hacer sin esta función
static void void_scommand_destroy(void* self) {
    scommand self2 = (scommand) self;
    scommand_destroy(self2);
}

pipeline pipeline_destroy(pipeline self){
    assert(self != NULL);

    g_slist_free_full(self->scmds, void_scommand_destroy); self->scmds = NULL;
    free(self); self = NULL;

    assert(self == NULL);
    return self;
}

void pipeline_push_back(pipeline self, scommand sc){
    // El TAD se apropia del comando
    assert(
        self != NULL &&
        sc != NULL
    );

    self->scmds = g_slist_append(self->scmds, sc);

    assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self){
}

void pipeline_set_wait(pipeline self, const bool w){
    assert(self != NULL);

    self->wait = w;
}

bool pipeline_is_empty(const pipeline self){
    assert(self != NULL);

    return(self->scmds == NULL);
}

unsigned int pipeline_length(const pipeline self){
    assert(self != NULL);

    return(g_slist_length(self->scmds));
}

scommand pipeline_front(const pipeline self){
    assert(self != NULL && !pipeline_is_empty(self));

    scommand result = pipeline_get_command(self->scmds, 0);

    assert(result!=NULL);

    return result;
}

static scommand pipeline_get_command(const pipeline self, unsigned int n){
    assert(self != NULL);

    return g_slist_nth(self->scmds, n);
}

bool pipeline_get_wait(const pipeline self){
    assert(self != NULL);

    return self->wait;
}

char * pipeline_to_string(const pipeline self){
    assert(self != NULL);

    char * result = "";

    for(unsigned int i=0; i < pipeline_length(self); i++){
        char * myChar = scommand_to_string(pipeline_get_command(self, i));
        if(i != 0){
            result = str_concat(result, " | ");
        }
        result = str_concat(result, myChar);
    }

    assert(
        pipeline_is_empty(self) ||
        pipeline_get_wait(self) || strlen(result)>0
    );

    return result;
}
