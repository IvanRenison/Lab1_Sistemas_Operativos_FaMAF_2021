#include <assert.h>
#include <fcntl.h> // open
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "command.h"
#include "execute.h"

void execute_pipeline(pipeline apipe) {
    assert(apipe != NULL);
    // Checkear el tamaño de la pipe
    unsigned int N = pipeline_length(apipe);
    // Un solo comando
    if (N == 1) {
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

/* Pone, si está seeteado, el archivo de redireción de entrada en el stdin,
 * si no está seeteado no hace nada
 *
 * Requires: cmd != NULL
 *
 */
/* static int change_file_descriptor_in(scommand cmd) {
    assert(cmd != NULL);

    char* redir_in = scommand_get_redir_in(cmd);
    // si la redireción de entrada no está seeteada, scommand_get_redir_in
    // devuelve NULL
    if (redir_in != NULL) {
        int file_redir_in = open(redir_in, O_RDONLY);
        // Si el archivo no existe, o hay algún otro tipo de error, open retorna
        // -1, si no retorna el descriptor del archivo
        if (file_redir_in == -1) {
            // En caso de error, open seetea el mansaje de perror
            perror(redir_in);
            return (-1);
        }

        int dup2_res = dup2(file_redir_in, STDIN_FILENO);
        // Si dup2 falla, retorna -1
        if (dup2_res == -1) {
            // dup2 no suele fallar, pero podría llegar a hacerlo
            // Si lo hace, dup2 seetea el mensaje de perror
            perror("");
            return (-1);
        }
    }
    return (0);
} */

/* Pone, si está seeteado, el archivo de redireción de salida en el stdout,
 * si el archivo en existe lo crea
 * Si la redireción de salida no está seeteada no hace nada
 *
 * Requires: cmd != NULL
 *
 */
/* static int change_file_descriptor_out(scommand cmd) {
    assert(cmd != NULL);

    char* redir_out = scommand_get_redir_out(cmd);
    // si la redireción de entrada no está seeteada, scommand_get_redir_in
    // devuelve NULL
    if (redir_out != NULL) {

        / *   open como segundo parametro toma flags, en este caso tiene los
           flags O_WRONLY que hace que el archivo se abra en solo escritura, y
           O_CREAT que hace que si el archivo no existe, se cree. En el caso de
           estar seeteado O_CREAT, open toma un tercer parametro de flags de
           cración del archivo, en este caso están puestos los flags:
               S_IRUSR: user has read permission
               S_IWUSR: user has write permission
             No sabemos con exactitud que pasa si se pone el O_CREAT pero no se
           pasa un tercer parametro * /
        
        int file_redir_out =
            open(redir_out, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

        // Si hay algún error devuelve -1, en caso de que no exista el archivo,
        // lo crea si no hay error retorna el descriptor del archivo
        if (file_redir_out == -1) {
            // En caso de error, open seetea el mansaje de perror
            perror(redir_out);
            return (-1);
        }

        int dup2_res = dup2(file_redir_out, STDOUT_FILENO);
        // Si dup2 falla, retorna -1
        if (dup2_res == -1) {
            // dup2 no suele fallar, pero podría llegar a hacerlo
            // Si lo hace, dup2 seetea el mensaje de perror
            perror("");
            return (-1);
        }
    }
    return (0);
} */

/* Ejecuta un comando en el mismo proseso, es decir sin hacer fork
 * puede modificar cmd, pero no destruirlo
 * Si la llamada sale bien no se retorna, si la llamada sale mal, se retorna el
 * código de error
 *
 * Requires: cmd != NULL && !scommand_is_empty(cmd)
 *
 */
int scommand_exec(scommand cmd) {
    assert(cmd != NULL && !scommand_is_empty(cmd));

    char** argv = scommand_to_argv(cmd);
    // argv != NULL  por poscondición de scommand_to_argv
    int ret_code = execvp(argv[0], argv);

    /* Si execvp falla (y por ende retorna) se imprime un mensaje
      y se libera la memoria */
    perror(argv[0]);

    unsigned int j = 0u;
    while (argv[j] != NULL) {
        free(argv[j]);
        j++;
    }
    free(argv);

    return (ret_code);
}
