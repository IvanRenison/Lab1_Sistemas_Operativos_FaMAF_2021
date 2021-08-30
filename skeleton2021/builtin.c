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
    if(length == 2u){
        char * input_path = scommand_get_nth(cmd, 1u);
        int ret_code = 0;

        /* Si el argumento de chdir comienza con / el path se toma desde equipo
           (ósea como path absoluto) y si empieza con ./ o sin nada se toma desde
           el directorio actual. También, chdir acepta .. para ir un directorio
           para arriba.
           
           Lo que chdir no asepta es usar ~ para que el path se tome desde el home,
           implementarlo es bastante complicado, hay una implementación hecha, pero
           está comentada porque no tiene en cuenta el caso en el que alguien tenga
           una carpeta cuyo nombre que empiece con ~
        */
        /* Siempre vale que input_path != NULL ya que todos los argumentos de un
           scommand son distintos de NULL
         */
/*         if(input_path[0] == '~') {
            char * home_path = getenv("HOME");
            home_path = str_concat(home_path, input_path);

            ret_code = chdir(home_path);

            free(home_path); home_path = NULL;
        }
        else { */
            ret_code = chdir(input_path);
//        }
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
    else { // En este caso length == 1u
        printf("mybash: cd: sin argumentos\n");
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
