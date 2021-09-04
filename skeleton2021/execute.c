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

/* Pone, si está seeteado, el archivo de redireción de entrada en el stdin,
 * si no está seeteado no hace nada
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

        int res_close = close(file_redir_in);
        if (res_close == -1) {
            // No debería fallar, PERO si falla...
            perror("");
            return (-1);
        }
    }
    return (0);
}

/* Pone, si está seeteado, el archivo de redirección de salida en el stdout,
 * si el archivo en existe lo crea
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

        /*   open como segundo parametro toma flags, en este caso tiene los
           flags O_WRONLY que hace que el archivo se abra en solo escritura, y
           O_CREAT que hace que si el archivo no existe, se cree. En el caso de
           estar seeteado O_CREAT, open toma un tercer parametro de flags de
           cración del archivo, en este caso están puestos los flags:
               S_IRUSR: user has read permission
               S_IWUSR: user has write permission
             No sabemos con exactitud que pasa si se pone el O_CREAT pero no se
           pasa un tercer parametro */

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

        int res_close = close(file_redir_out);
        if (res_close == -1) {
            // No debería fallar, PERO si falla...
            perror("");
            return (-1);
        }
    }
    return (0);
}

/* Ejecuta un comando como comando externo en el mismo proseso, es decir sin hacer
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
int scommand_exec_external(scommand cmd);
int scommand_exec_external(scommand cmd) {
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
 * En el caso de ser externo solo retorno si hay un error, y en ese caso,
 * devuelve -1. En caso se ser un interno, retorna 0.
 * Puede modificar cmd, pero no destruirlo
 * 
 * En caso de fallar la llamada al programa, los descriptores de archivo pueden quedar
 * cambiados por los de redirección de entrada y de salida
 *
 * Requires: cmd != NULL && !scommand_is_empty(cmd)
 */
static int scommand_exec(scommand cmd) {
    assert(cmd != NULL && !scommand_is_empty(cmd));
    int ret_code = 0;
    if (builtin_scommand_is_internal(cmd)) {
        builtin_scommand_exec(cmd);
        ret_code = 0;
    } else {
        ret_code = scommand_exec_external(cmd);
    }

    return (ret_code);
}

/* Ejecuta un pipeline no vacío, creando procesos hijos incluso para los comandos internos
 * En caso de algún error deja de ejecutar.
 * Modifica apipe, ya que le va sacando los comandos, y si no hay ningún error,
 * lo deja vacío
 *
 * Requires: apipe != NULL && !pipeline_is_empty(apipe)
 *
 */
/*static void execute_multiple_pipelines(pipeline apipe) {
    assert(apipe != NULL && !pipeline_is_empty(apipe));

    unsigned int cantidadDeHijos = 0u;
    bool error_flag = false;
    // Variable para saber si hubo un error
    // Si está en false es porque no hubo errores, si se vuelve true es porque si hubo

    while (!error_flag && pipeline_length(apipe) > 1) {
        scommand comando = pipeline_front_and_pop(apipe);

        if (!scommand_is_empty(comando)) {
            int pipefd[2];
            int res_pipe = pipe(pipefd);
            if (res_pipe < 0) {
                perror("pipe");
                error_flag = true;
            }
            else {
                int punta_lectura = pipefd[0];
                int punta_escritura = pipefd[1];

                pid_t pid = fork();
                if (pid < 0) {
                    perror("fork");
                    error_flag = true;
                } else if (pid == 0) {
                    // El proceso hijo
                    close(punta_lectura);
                    dup2(punta_escritura, STDOUT_FILENO);
                    close(punta_escritura);
                    scommand_exec(comando);
                    // Esto no se debería ejecutar
                    _exit(EXIT_FAILURE);
                }
                else {
                    // El proceso padre
                    cantidadDeHijos++;
                    // Se deja la punta de lectura modificada para el proximo ciclo
                    close(punta_escritura);
                    dup2(punta_lectura, STDIN_FILENO);
                    close(punta_lectura);
                }
            }
        }
        comando = scommand_destroy(comando);
    }
    if (!error_flag) {
        // Acá queda un solo comando
        // Ya que el while se ejecuto hasta que pipeline_length(apipe) = 1
        scommand comando = pipeline_front_and_pop(apipe);
        if (!scommand_is_empty(comando)) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                error_flag = true;
            } else if (pid == 0) {
                // EL proceso hijo
                scommand_exec(comando);
                // Esto no se debería ejecutar
                _exit(EXIT_FAILURE);
            }
            else {
                // El proceso padre
                cantidadDeHijos++;
            }
        }
        comando = scommand_destroy(comando);
    }

    // Se espera a que todos los proseso hijos terminen
    while (cantidadDeHijos > 0) {
        wait(NULL);
        cantidadDeHijos--;
    }
}*/

void execute_pipeline(pipeline p) {
    int numberOfPipes = pipeline_length(p) - 1;

    int status;
    int i = 0;
    pid_t pid;

    int* pipefds = calloc(2 * numberOfPipes, sizeof(int));

    for (i = 0; i < (numberOfPipes); i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("Pipe failed");
        }
    }

    int j = 0;
    while (!pipeline_is_empty(p)) {
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            _exit(-2);
        } else if (pid == 0) {

            if (pipeline_length(p) > 1) {
                if (dup2(pipefds[j + 1], STDOUT_FILENO) < 0) {
                    perror("dup2");
                }
            }

            if (j != 0) {
                if (dup2(pipefds[j - 2], STDIN_FILENO) < 0) {
                    perror("dup2");
                }
            }

            for (i = 0; i < 2 * numberOfPipes; i++) {
                close(pipefds[i]);
            }

            if (scommand_exec(pipeline_front(p)) == -1) {
                _exit(-2);
            }
        }
        if (!pipeline_is_empty(p)) {
            pipeline_pop_front(p);
        }
        j += 2;
    }

    for (i = 0; i < 2 * numberOfPipes; i++) {
        close(pipefds[i]);
    }

    if (!pipeline_get_wait(p)) {
        for (i = 0; i <= numberOfPipes; i++) {
            waitpid(-1, &status, WNOHANG);
        }
    } else {
        for (i = 0; i <= numberOfPipes; i++) {
            wait(&status);
        }
    }
    free(pipefds);
}
