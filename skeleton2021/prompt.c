#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "prompt.h"

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

    printf("%s@%s/mybash>%s$ ", password->pw_name, hostname, workdir);
    fflush(stdout);
}
