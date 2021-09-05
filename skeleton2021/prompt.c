#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "prompt.h"

/* Para imprimir con color se usan códigos ANSI de la forma
   "\033[38;2;rojo;verde;azulm"
*/

#define ANSI_NOCOLOR "\033[0m" // Esto vuelve al color por defecto

#define ANSI_GREEN "\033[38;2;0;255;0m"
#define ANSI_BLUE "\033[38;2;14;112;255m"

void show_prompt(void) {
    char hostname[256];
    hostname[255] = '\0'; // El último caracter es NULL
    gethostname(hostname, 256);

    unsigned int user_id = geteuid();
    struct passwd* password =
        getpwuid(user_id); // struct passwd *getpwnam(const char *name);

    char workdir[2556];
    workdir[2555] = '\0';
    getcwd(workdir, 2556);

    printf(ANSI_GREEN "%s@%s" ANSI_NOCOLOR "/mybash>" ANSI_BLUE
                      "%s" ANSI_NOCOLOR "$ ",
           password->pw_name, hostname, workdir);
    /* Cuando se pone un color, lo que se sigue imprimiendo, se imprime con ese color,
       por ende, para usar el color por defecto hay que usar ANSI_NOCOLOR */
    fflush(stdout);
}
