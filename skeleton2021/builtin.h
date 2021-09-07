#ifndef _BUILTIN_H_
#define _BUILTIN_H_

#include <stdbool.h>

#include "command.h"

/* Variable global para volver true cuando el mybash se tiene que terminar
 */
bool exit_from_mybash;

/*
 * Indica si un "exit"
 *
 * REQUIRES: cmd != NULL
 */
bool builtin_scommand_is_exit(const scommand cmd);

/*
 * Indica si el comando es un "cd"
 *
 * REQUIRES: cmd != NULL
 */
bool builtin_scommand_is_cd(const scommand cmd);


/*
 * Indica si un comando es interno
 *
 * REQUIRES: cmd != NULL
 */
bool builtin_scommand_is_internal(const scommand cmd);

/*
 * Indica si el pipeline tiene un solo comando y ese comando es interno
 *
 * REQUIRES: pipe != NULL
 *
 */
bool builtin_scommand_is_single_internal(const pipeline pipe);

/*
 * Ejecuta un comando interno
 *
 * REQUIRES: cmd != NULL && builtin_scommand_is_internal(cmd)
 */
void builtin_scommand_exec(const scommand cmd);

/*
 * Ejecuta una pipeline con un único comando
 *
 * REQUIRES: pipe != NULL && builtin_scommand_is_single_internal(pipe)
 */
void builtin_single_pipeline_exec(const pipeline pipe);

#endif
