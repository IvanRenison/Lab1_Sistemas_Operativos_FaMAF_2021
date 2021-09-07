#include <assert.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "strextra.h"

/********** Funciones auxiliares **********/

/* Elimina el primer elemento de una lista no vacía, destrullendolo con
 * free_func Requires: xs != NULL && free_func != NULL
 */
static GSList* g_slist_tail_free_full(GSList* xs, GDestroyNotify free_func) {
    assert(xs != NULL && free_func != NULL);

    gpointer head = g_slist_nth_data(xs, 0u);

    GSList* result = g_slist_remove(xs, head);
    // head se vuelve una lista con un solo nodo

    free_func(head);

    return (result);
}

/********** COMANDO SIMPLE **********/

/* Estructura correspondiente a un comando simple.
 * Es una 3-upla del tipo ([char*], char* , char*).
 */
struct scommand_s {
    GSList* args;
    char* redir_in;
    char* redir_out;
};

scommand scommand_new(void) {
    scommand result = malloc(sizeof(struct scommand_s));
    result->args = NULL;
    result->redir_in = NULL;
    result->redir_out = NULL;

    assert(result != NULL && scommand_is_empty(result) &&
           scommand_get_redir_in(result) == NULL &&
           scommand_get_redir_out(result) == NULL);

    return result;
}

scommand scommand_destroy(scommand self) {
    assert(self != NULL);

    g_slist_free_full(self->args, free);
    self->args = NULL;

    free(self->redir_in);
    self->redir_in = NULL;
    free(self->redir_out);
    self->redir_out = NULL;

    free(self);
    self = NULL;

    assert(self == NULL);

    return self;
}

void scommand_push_back(scommand self, char* argument) {
    assert(self != NULL && argument != NULL);

    self->args = g_slist_append(self->args, argument);

    assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self) {
    assert(self != NULL && !scommand_is_empty(self));

    self->args = g_slist_tail_free_full(self->args, free);
}

void scommand_set_redir_in(scommand self, char* filename) {
    assert(self != NULL);

    self->redir_in = filename;
}

void scommand_set_redir_out(scommand self, char* filename) {
    assert(self != NULL);

    self->redir_out = filename;
}

bool scommand_is_empty(const scommand self) {
    assert(self != NULL);

    return (self->args == NULL);
}

unsigned int scommand_length(const scommand self) {
    assert(self != NULL);

    unsigned int length = g_slist_length(self->args);
    // g_slist_length devuelve un guint que es lo mismo que un unsigned int

    assert((length == 0) == scommand_is_empty(self));
    return length;
}

char* scommand_front(const scommand self) {
    assert(self != NULL && !scommand_is_empty(self));

    char* result = g_slist_nth_data(self->args, 0u);

    assert(result != NULL);
    return result;
}

char* scommand_front_and_pop(scommand self) {
    assert(self != NULL && !scommand_is_empty(self));

    char* result = g_slist_nth_data(self->args, 0u);

    self->args = g_slist_remove(self->args, result);
    // g_slist_remove libera el nodo, pero no el elemento

    assert(result != NULL);
    return (result);
}

char* scommand_get_nth(scommand self, unsigned int n) {
    assert(self != NULL && n < scommand_length(self));

    char* result = g_slist_nth_data(self->args, n);

    assert(result != NULL);
    return result;
}

char* scommand_get_redir_in(const scommand self) {
    assert(self != NULL);

    return (self->redir_in);
}

char* scommand_get_redir_out(const scommand self) {
    assert(self != NULL);

    return (self->redir_out);
}

char** scommand_to_argv(scommand self) {
    assert(self != NULL);

    unsigned int n = scommand_length(self);
    char** argv = calloc(sizeof(char*), n + 1);

    for (unsigned int j = 0; j < n; j++) {
        char* arg = scommand_front_and_pop(self);
        argv[j] = arg;

        assert(argv[j] != NULL);
        // Este assert verifica la parte de ⟨∀j ∈ 0..n-1 : argv[i] != NULL⟩
    }
    argv[n] = NULL;

    assert(self != NULL && scommand_is_empty(self) && argv != NULL &&
           argv[n] == NULL);
    return (argv);
}

char* scommand_to_string(const scommand self) {
    assert(self != NULL);

    GSList* xs = self->args;
    char* result = strdup("");

    if (xs != NULL) {
        result = str_concat(result, xs->data);
        xs = g_slist_next(xs);
        while (xs != NULL) {
            result = str_concat(result, " ");
            result = str_concat(result, g_slist_nth_data(xs, 0u));
            xs = g_slist_next(xs);
        }
    }

    if (self->redir_out != NULL) {
        result = str_concat(result, " > ");
        result = str_concat(result, self->redir_out);
    }

    if (self->redir_in != NULL) {
        result = str_concat(result, " < ");
        result = str_concat(result, self->redir_in);
    }

    assert(scommand_is_empty(self) || scommand_get_redir_in(self) == NULL ||
           scommand_get_redir_out(self) == NULL || strlen(result) > 0);

    return (result);
}

/* Agrega en chars una representación del comando self
 * para hacerlo realoca la memoría
 * Requires: chars != NULL && self != NULL
 */
static char* append_scommand_to_string(char* chars, const scommand self) {
    assert(chars != NULL && self != NULL);

    char* myChar = scommand_to_string(self);
    chars = str_concat(chars, myChar);
    free(myChar);
    myChar = NULL;

    return (chars);
}

/********** COMANDO PIPELINE **********/

/* Estructura correspondiente a un comando pipeline.
 * Es un 2-upla del tipo ([scommand], bool)
 */
struct pipeline_s {
    GSList* scmds;
    bool wait;
};

pipeline pipeline_new(void) {
    pipeline result = malloc(sizeof(struct pipeline_s));
    result->scmds = NULL;
    result->wait = true;

    assert(result != NULL && pipeline_is_empty(result) &&
           pipeline_get_wait(result));
    return result;
}

/* g_slist_free_full necesita una función que devuelva void
 * para liberar una lista de scommand se puede usar esta función
 */
static void void_scommand_destroy(void* self) {
    scommand self2 = self;
    scommand_destroy(self2);
}

pipeline pipeline_destroy(pipeline self) {
    assert(self != NULL);

    g_slist_free_full(self->scmds, void_scommand_destroy);
    self->scmds = NULL;
    free(self);
    self = NULL;

    assert(self == NULL);
    return self;
}

void pipeline_push_back(pipeline self, scommand sc) {
    // El TAD se apropia del comando
    assert(self != NULL && sc != NULL);

    self->scmds = g_slist_append(self->scmds, sc);

    assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self) {
    assert(self != NULL && !pipeline_is_empty(self));

    self->scmds = g_slist_tail_free_full(self->scmds, void_scommand_destroy);
}

void pipeline_set_wait(pipeline self, const bool w) {
    assert(self != NULL);

    self->wait = w;
}

bool pipeline_is_empty(const pipeline self) {
    assert(self != NULL);

    return (self->scmds == NULL);
}

unsigned int pipeline_length(const pipeline self) {
    assert(self != NULL);

    return g_slist_length(self->scmds);
}

scommand pipeline_front(const pipeline self) {
    assert(self != NULL && !pipeline_is_empty(self));

    scommand result = g_slist_nth_data(self->scmds, 0u);

    assert(result != NULL);

    return result;
}

scommand pipeline_front_and_pop(pipeline self) {
    assert(self != NULL && !pipeline_is_empty(self));

    scommand result = g_slist_nth_data(self->scmds, 0u);

    self->scmds = g_slist_remove(self->scmds, result);
    // g_slist_remove libera el nodo, pero no el elemento

    assert(result != NULL);
    return (result);
}

bool pipeline_get_wait(const pipeline self) {
    assert(self != NULL);

    return self->wait;
}

char* pipeline_to_string(const pipeline self) {
    assert(self != NULL);

    GSList* commands = self->scmds;
    char* result = strdup("");

    if (commands != NULL) {
        result =
            append_scommand_to_string(result, g_slist_nth_data(commands, 0u));
        commands = g_slist_next(commands);

        while (commands != NULL) {
            result = str_concat(result, " | ");
            result = append_scommand_to_string(result,
                                               g_slist_nth_data(commands, 0u));
            commands = g_slist_next(commands);
        }

        if (!pipeline_get_wait(self)) {
            result = str_concat(result, " &");
        }
    }

    assert(pipeline_is_empty(self) || pipeline_get_wait(self) ||
           strlen(result) > 0);

    return result;
}
