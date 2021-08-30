#include <assert.h>
#include <stdio.h>

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
}
