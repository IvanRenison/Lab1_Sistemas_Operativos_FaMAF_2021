#include <assert.h>
#include <fcntl.h> // open
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtin.h"
#include "command.h"
#include "execute.h"

// typedef

/* Sinonimo de tipo para los descriptores de archivo
 *
 * Los descriptores de archivo habitualmente son ints, pero nosotros vamos
 * a usar el tipo fd_t para distinguirlos mejor, y que el código sea mas claro
 *
 * El nombre viene de "file descriptor type" ("tipo descriptor de archivo" en español)
 */
typedef int fd_t;

/* Pone, si existe, el archivo de redirección de entrada en el stdin,
 * si no existe no hace nada
 * Returns: 0 si la operación fue exitosa
 *          1 si la operación falló
 *
 * Requires: cmd != NULL
 *
 */
static int change_file_descriptor_in(scommand cmd) {
    assert(cmd != NULL);

    char* redir_in = scommand_get_redir_in(cmd);
    // si la redirección de entrada no está seeteada, scommand_get_redir_in devuelve NULL
    if (redir_in != NULL) {
        fd_t file_redir_in = open(redir_in, O_RDONLY);
        // Si el archivo no existe, o hay algún otro tipo de error, open retorna
        // -1, si no retorna el descriptor del archivo
        if (file_redir_in == -1) {
            // En caso de error, open seetea el mansaje de perror
            perror(redir_in);
            return (-1);
        }

        fd_t dup2_res = dup2(file_redir_in, STDIN_FILENO);
        // Si dup2 falla, retorna -1
        if (dup2_res == -1) {
            // dup2 no suele fallar, pero podría llegar a hacerlo
            // Si lo hace, dup2 seetea el mensaje de perror
            perror("dup2");
            return (-1);
        }

        int res_close = close(file_redir_in);
        if (res_close == -1) {
            // No debería fallar, PERO si falla...
            perror("dup2");
            return (-1);
        }
    }
    return (0);
}

/* Pone, si existe, el archivo de redirección de salida en el stdout,
 * si el archivo no existe lo crea.
 * Si la redirección de salida no está seeteada no hace nada
 * Returns: 0 si la operación fue exitosa
 *          1 si la operación falló
 *
 * Requires: cmd != NULL
 *
 */
static int change_file_descriptor_out(scommand cmd) {
    assert(cmd != NULL);

    char* redir_out = scommand_get_redir_out(cmd);
    // si la redirección de salida no está seeteada, scommand_get_redir_out devuelve NULL
    if (redir_out != NULL) {

        /*  open como segundo parametro toma flags, en este caso tiene los
           flags O_WRONLY que hace que el archivo se abra en solo escritura, y
           O_CREAT que hace que si el archivo no existe, se cree. En el caso de
           estar seeteado O_CREAT, open toma un tercer parametro de flags de
           cración del archivo, en este caso están puestos los flags:
               S_IRUSR: user has read permission
               S_IWUSR: user has write permission
             */

        fd_t file_redir_out =
            open(redir_out, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

        // Si hay algún error devuelve -1, en caso de que no exista el archivo
        // lo crea, si no hay error retorna el descriptor del archivo
        if (file_redir_out == -1) {
            // En caso de error, open seetea el mansaje de perror
            perror(redir_out);
            return (-1);
        }

        fd_t dup2_res = dup2(file_redir_out, STDOUT_FILENO);
        // Si dup2 falla, retorna -1
        if (dup2_res == -1) {
            // dup2 no suele fallar, pero podría llegar a hacerlo
            // Si lo hace, dup2 seetea el mensaje de perror
            perror("dup2");
            return (-1);
        }

        int res_close = close(file_redir_out);
        if (res_close == -1) {
            // No debería fallar, PERO si falla...
            perror("close");
            return (-1);
        }
    }
    return (0);
}

/* Ejecuta un comando como comando externo en el mismo proceso, es decir sin hacer
 * fork, redirigiendo el stdin y el stdout si están seeteados
 * Puede modificar cmd, pero no destruirlo
 * Si la llamada sale bien no se retorna, si la llamada sale mal, si hay algún
 * problema con la redirección de entrada o de salida se retorna -1, y si la llamada
 * al programa falla se retorna el código de error
 * En caso de fallar la llamada al programa, los descriptores de archivo quedan cambiados
 *
 * Requires: cmd != NULL && !scommand_is_empty(cmd)
 *
 */
static int scommand_exec_external(scommand cmd) {
    assert(cmd != NULL && !scommand_is_empty(cmd));

    // Se cambia stdin por el archivo de redirección de entrada, si es que está seeteado
    int exit_redir_in = change_file_descriptor_in(cmd);
    if (exit_redir_in != 0) {
        return (-1);
    }

    // Se cambia stdout por el archivo de redirección de salida, si es que está seeteado
    int exit_redir_out = change_file_descriptor_out(cmd);
    if (exit_redir_out != 0) {
        return (-1);
    }

    char** argv = scommand_to_argv(cmd);
    if (argv == NULL) {
        // En caso de que scommand_to_argv falle
        return (-1);
    }
    // argv != NULL  por poscondición de scommand_to_argv
    int ret_code = execvp(argv[0], argv);

    /* Si execvp falla (y por ende retorna) se imprime un mensaje
      y se libera la memoria */
    perror(argv[0]);
    ret_code = ret_code;

    unsigned int j = 0u;
    while (argv[j] != NULL) {
        free(argv[j]);
        argv[j] = NULL;
        j++;
    }
    free(argv);
    argv = NULL;

    return ret_code;
}

/* Ejecuta un comando, tanto si es interno como si es externo
 * En el caso de ser externo solo retorna si hay un error, y en ese caso,
 * devuelve -1. En caso se ser un interno, retorna 0.
 * Puede modificar cmd, pero no destruirlo
 * 
 * En caso de fallar la llamada al programa, los descriptores de archivo pueden quedar
 * cambiados por los de redirección de entrada y de salida
 *
 * Requires: cmd != NULL
 */
static int scommand_exec(scommand cmd) {
    assert(cmd != NULL);
    int ret_code = 0;
    if (builtin_scommand_is_internal(cmd)) {
        builtin_scommand_exec(cmd);
        ret_code = 0;
    } else if (!scommand_is_empty(cmd)) {
        ret_code = scommand_exec_external(cmd);
    }

    return (ret_code);
}

/* Ejecuta un pipeline de un solo comando tanto si es interno como si es externo
 * en caso de ser externo hace fork pero en caso de ser interno no.
 * Retorna la cantidad de hijos creados (0 o 1)
 *
 * Requires: apipe != NULL && pipeline_length(apipe) == 1
 */
static unsigned int single_command(pipeline apipe) {
    assert(apipe != NULL && pipeline_length(apipe) == 1);

    unsigned int child_processes_running = 0u;

    if (builtin_scommand_is_single_internal(apipe)) {
        // Caso en el que comando es interno
        builtin_single_pipeline_exec(apipe);
    } else {
        //Caso en el que el comando es externo y se debe hacer fork()
        pid_t pid = fork();
        if (pid < 0) {
            // En caso de error
            perror("fork");
            return child_processes_running;
        } else if (pid == 0) {
            // El hijo
            int res_exec = scommand_exec(pipeline_front(apipe));
            _exit(res_exec);
        } else {
            // El padre
            // Se cuenta un hijo
            child_processes_running++;
        }
    }

    return child_processes_running;
}

/* Ejecutá un pipeline de multiples comandos (2 o mas) haciendo fork para cada comando
 * (incluso para los internos) y retorna la cantidad de hijos creados
 * 
 * Puede modificar apipe pero no destruirlo, en caso de que no haya ningún error,
 * deja vacio a apipe
 *
 * Requires: apipe != NULL && pipeline_length(apipe) >= 2
 * 
 * Ensures: apipe != NULL
 */
static unsigned int multiple_commands(pipeline apipe) {
    assert(apipe != NULL && pipeline_length(apipe) >= 2u);

    unsigned int child_processes_running = 0u;

    unsigned int numberOfPipes = pipeline_length(apipe) - 1u;
    // pipeline_length(apipe) >= 2  ⇒  numberOfPipes >= 1
    bool error_flag = false;

    // Se asigna la cantidad de memoria necesaria para todos los pipes
    fd_t* pipesfd = calloc(2 * numberOfPipes, sizeof(fd_t));
    if (pipesfd == NULL) {
        // En caso de que calloc falle
        // Se imprime mensaje de error
        perror("calloc");
        // Se retorna
        return child_processes_running;
    }

    // Se abren todos los pipes que se van a necesitar para la ejecucion
    for (unsigned int i = 0u; i < numberOfPipes; i++) {
        int res_pipe = pipe(pipesfd + i * 2);
        if (res_pipe < 0) {
            // En caso de error de pipe
            perror("pipe");
            // se cierran los pipes que ya se abrieron
            for (unsigned int j = 0u; j < 2u * i; j++) {
                close(pipesfd[j]);
            }
            // se libera la memoria
            free(pipesfd);
            pipesfd = NULL;
            return child_processes_running;
        }
    }

    unsigned int j = 0;
    // Caso en el que haya un pipeline multiple
    while (!pipeline_is_empty(apipe) && !error_flag) {
        pid_t pid = fork();
        if (pid < 0) {
            //Caso de que el fork falle
            perror("error");
            //Si el fork falla se sale del ciclo para liberar la memoria y esperar a los
            //hijos que ya se ejecutaron
            error_flag = true;
        }
        if (pid == 0) {
            // El hijo

            //Si no es el ultimo comando
            if (pipeline_length(apipe) > 1) {
                fd_t res_dup = dup2(pipesfd[j + 1], STDOUT_FILENO);
                if (res_dup < 0) {
                    perror("dup");
                    _exit(EXIT_FAILURE);
                }
            }

            // Si no es el primer comando
            if (j != 0u) {
                /* j se va incrementando de a 2, y nunca se decrementa,
                   por ende j >= 2 */
                fd_t res_dup = dup2(pipesfd[j - 2u], STDIN_FILENO);
                if (res_dup < 0) {
                    perror("dup");
                    _exit(EXIT_FAILURE);
                }
            }

            // Se cierran todos los file descriptors
            for (unsigned int i = 0u; i < 2u * numberOfPipes; i++) {
                close(pipesfd[i]);
            }

            int res_exec = scommand_exec(pipeline_front(apipe));

            /* En caso de que el comando haya sido interno, o la ejecución haya
               fallado, se termina el hijo */
            _exit(res_exec);

        } else if (pid > 0) {
            // El padre
            // Elimina un comando del pipe y aumenta el contador de procesos hijos
            // ejecutandose
            pipeline_pop_front(apipe);
            j = j + 2u;
            child_processes_running++;
        }
    }

    // Se cierran los descriptores de archivo
    for (unsigned int i = 0u; i < 2u * numberOfPipes; i++) {
        close(pipesfd[i]);
    }

    // Se libera la memoria asignada
    free(pipesfd);
    pipesfd = NULL;

    return child_processes_running;
}

/* Ejecuta un pipeline, haciendo fork para cada comando
 * y retorna la cantidad de hijos creados
 * 
 * Puede modificar apipe pero no destruirlo
 * 
 * Requires: apipe != NULL
 * 
 * Ensures: apipe != NULL
 */
static unsigned int execute_pipeline_foreground(pipeline apipe) {
    assert(apipe != NULL);

    unsigned int length = pipeline_length(apipe);
    unsigned int child_processes_running = 0u;

    if (length == 1u) {
        child_processes_running = single_command(apipe);
    } else if (length >= 2u) {
        child_processes_running = multiple_commands(apipe);
    }
    // En el caso de que apipe esté vacio no se hace nada

    assert(apipe != NULL);

    return child_processes_running;
}

/* Ejecuta los pipelines haciendo llamadas a las diferentes funciones
 * que ejecutan pipelines simples, en el caso de que un pipeline tenga comandos 
 * multiples y se ejecute en background hace un fork y una llamada a
 * execute_pipeline_foreground, de esta forma los procesos que ejecutan los comandos 
 * son hijos del hijo, de esta forma podemos hacer que el proceso hijo haga exit para que
 * no espere a los hijos y de esta forma se corran los procesos en background, y ademas
 * quedan sus hijos como huerfanos por lo que el proceso padre se encarga de recogerlos, 
 * de esta manera evitamos los procesos zombies.    
*/

void execute_pipeline(pipeline p) {
    assert(p != NULL);

    if (pipeline_get_wait(p)) {
        // Se ejecutan todos los comandos
        unsigned int child_processes_running = execute_pipeline_foreground(p);

        // Se espera a que todos los hijos terminen
        while (child_processes_running > 0u) {
            wait(NULL);
            child_processes_running--;
        }
    } else {
        // Hay que correrlo en modo background
        pid_t pid = fork();
        if (pid < 0) {
            // Caso de que el fork falle
            perror("fork");
        } else if (pid == 0) {
            // El proceso hijo

            // Se conecta el stdin del hijo a un archivo vacio
            /* Como archivo vacio se usa una punta de lectura de pipe
               con punta de escritura cerrada */
            fd_t pipefds[2];
            int res_pipe = pipe(pipefds);
            if (res_pipe < 0) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
            fd_t punta_lectura = pipefds[0];
            fd_t punta_escritura = pipefds[1];

            close(punta_escritura);

            fd_t res_dup2 = dup2(punta_lectura, STDIN_FILENO);
            if (res_dup2 < 0) {
                perror("perror ");
                exit(EXIT_FAILURE);
            }

            // Ejecuta todos los comandos del pipeline
            execute_pipeline_foreground(p);

            // Y termina para que los hijos pasen a ser hijos del sistema
            exit(EXIT_SUCCESS);
        } else {
            // El proceso padre
            // Espera a que el hijo termine de crear todos los procesos
            wait(NULL);
        }
    }
}
