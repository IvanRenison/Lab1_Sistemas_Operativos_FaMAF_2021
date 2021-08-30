#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "command.h"
#include "strextra.h"

// exit

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


// cd

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
    if(length == 2u || length == 1u){
        /* Si el argumento de chdir comienza con / el path se toma desde equipo
           (ósea como path absoluto) y si empieza con ./ o sin nada se toma desde
           el directorio actual. También, chdir acepta .. para ir un directorio
           para arriba.
           
           En bash ademas de poderse usar esos comienzos, se puede usar ~ para
           que el path sea desde el home, en este mybash eso no está soportado
        */
        int ret_code = 0;

        if(scommand_length(cmd) > 1u && strcmp(scommand_get_nth(cmd, 1u), "~")){
            char * path = scommand_get_nth(cmd, 1u);
            if(strlen(path) > 1u){
                if(path[0] == '~'){
                    char * relative_path = path + sizeof(char);
                    char * full_path = strmerge(getenv("HOME"), relative_path);
                    ret_code = chdir(full_path);
                } else {
                    ret_code = chdir(path);
                }
            }
        } else {
            ret_code = chdir(getenv("HOME"));
        }

        if(ret_code != 0){
            /* La función chdir deja un mensaje en algún lado, con perror se puede
               imprimir el último mensaje, por lo cuál, en caso de error se la usa.
               perror toma un string, e imprime primero ese string, y después el
               mensaje de error. man perror para mas información
            */
            perror("mybash: cd: ");
        }
    }
    else if(length > 2u){
        printf("mybash: cd: demasiados argumentos\n");
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

    scommand cmd = pipeline_front(pipe);
    builtin_scommand_exec(cmd);
}
