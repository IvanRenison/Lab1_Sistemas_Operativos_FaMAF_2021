#include <stdio.h> 
#include <glib.h>

#include "command.h"


/********** COMANDO SIMPLE **********/

/* Estructura correspondiente a un comando simple.
 * Es una 3-upla del tipo ([char*], char* , char*).
 */

struct scommand_s {
    GSList *args;
    char * redir_in;
    char * redir_out;
};


scommand scommand_new(void){
	scommand new_command = malloc(sizeof(struct scommand_s));
	new_command->args = NULL;
	new_command->redir_in = NULL;
	new_command->redir_out = NULL;
    return new_command;    
}

scommand scommand_destroy(scommand self){
	return NULL;
}

void scommand_push_back(scommand self, char * argument){
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


