#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "command.h"

bool builtin_scommand_is_exit(const scommand cmd){
    assert(cmd != NULL);
    
    return strcmp(scommand_front(cmd), "exit") == 0;
}

/*
 * Ejecuta el comando interno exit
 *
 * REQUIRES: cmd != NULL && builtin_scommand_is_exit
 */
static void builtin_run_exit(const scommand cmd){
    assert(cmd != NULL && builtin_scommand_is_exit(cmd));

    exit(EXIT_SUCCESS); // ????????????????????????????????????????
}

bool builtin_scommand_is_cd(const scommand cmd){
    assert(cmd != NULL);
    return strcmp(scommand_front(cmd), "cd") == 0;
    // Si son iguales strcmp devuelve 0
}

/*
 * Ejecuta el comando interno cd
 *
 * REQUIRES: cmd != NULL && builtin_scommand_is_cd(cmd)
 */
static void builtin_run_cd(const scommand cmd){
    assert(cmd != NULL && builtin_scommand_is_cd(cmd));
    unsigned int length = scommand_length(cmd);
    if(length == 2u){
        char * path = scommand_get_nth(cmd, 1u);
        int ret_code = chdir(path);
        if(ret_code != 0){
            // Se podría tratar de ver porque el error
            printf("mybash: cd: ERROR\n");
        }
    }
    else if(length > 2u){
        printf("mybash: cd: demasiados argumentos\n");
    }
    else{
        // En caso de 1 solo argumento (solo el cd) se va al home
        printf("cd solo\n");
        chdir("'/");
    }
}

bool builtin_scommand_is_internal(const scommand cmd){
    assert(cmd != NULL);
    return builtin_scommand_is_exit(cmd) || builtin_scommand_is_cd(cmd);
}

bool builtin_scommand_is_single_internal(const pipeline pipe){
    assert(pipe != NULL);
    return
        pipeline_length(pipe) == 1 &&
        builtin_scommand_is_internal(pipeline_front(pipe));
}

void builtin_scommand_exec(const scommand cmd){
    assert(cmd != NULL && builtin_scommand_is_internal(cmd));
    if(builtin_scommand_is_cd(cmd)){
        builtin_run_cd(cmd);
    }
    else{
        builtin_run_exit(cmd);
    }
}

void builtin_single_pipeline_exec(const pipeline pipe){
    assert(pipe != NULL && builtin_scommand_is_single_internal(pipe));

}
