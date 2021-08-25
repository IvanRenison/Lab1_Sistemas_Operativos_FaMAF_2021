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

    g_slist_free(self->args);
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
	unsigned int length = 0u;
	if(!scommand_is_empty(self)){
		length = g_slist_length(self->args);
	}
	assert((length == 0) == (scommand_is_empty(self)));
    return length;
}

char * scommand_front(const scommand self){
    return NULL;
}

char * scommand_get_redir_in(const scommand self){
	assert(self != NULL);
    return (self->redir_in);
}

char * scommand_get_redir_out(const scommand self){
	assert(self != NULL);
    return (self->redir_out);
}

/* Concatena con un espacio en el medio al final de dest una
 * copia de src realocando dest
 * 
 * Requires: dest != NULL && src != NULL
 */
static char * str_concat_con_espacio(char * dest, const char * src){
	assert(dest != NULL && src != NULL);
	
	size_t dest_len = strlen(dest);
	size_t src_len = strlen(src);

	size_t res_len = dest_len + sizeof(' ') + src_len;

	dest = realloc(dest, res_len);

	dest = strcat(dest, " "); // adds a ' '
	dest = strcat(dest, src); // adds a copy of src

	return(dest);
}



char * scommand_to_string(const scommand self){
    
	
	

	return(NULL);
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


