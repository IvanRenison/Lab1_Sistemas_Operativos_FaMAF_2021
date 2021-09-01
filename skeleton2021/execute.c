#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "command.h"
#include "execute.h"

void execute_pipeline(pipeline apipe){
    assert(apipe != NULL);
    // Checkear el tamaño de la pipe
    unsigned int N = pipeline_length(apipe);
    // Un solo comando
    if(N == 1){
        // Ejecutar el comando
        printf("Ejecutar el comando...\n");
    } // Varios comandos
    else {
        // Recorrer los comandos y ejecutarlos
        // Checkear si es necesario esperar un comando
    }
    
    /*
    Estructura:
    
    if(builtin_scommand_is_single_internal(apipe)) {
        builtin_single_pipeline_exec(apipe);
    }
    else {
        while(!pipeline_is_empty(apipe)) {
            scommand cmd = pipeline_front(apipe);

            if(!builtin_scommand_is_internal(cmd)) {
                // frckear y crear el proseso hijo
            }

            pipeline_pop_front(apipe);
        }
        if(pipeline_get_wait(apipe)) {
            // Esperar a que terminen los hijos
        }
    }
    */
    
}





/* Ejecuta un comando en el mismo proseso, es decir sin hacer fork
 * puede modificar cmd, pero no destruirlo
 * Si la llamada sale bien no se retorna, si la llamada sale mal, se retorna el código de error
 * 
 * Requires: cmd != NULL && !scommand_is_empty(cmd)
 * 
 */
int scommand_exec(scommand cmd) {
    assert(cmd != NULL && !scommand_is_empty(cmd));

    char** argv = scommand_to_argv(cmd);

    int ret_code = execvp(argv[0], argv);

    return(ret_code);
}





