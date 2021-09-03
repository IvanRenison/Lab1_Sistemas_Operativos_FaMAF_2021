#include "strextra.h" /* Interfaz                           */
#include <assert.h>   /* assert()...                        */
#include <stdlib.h>   /* calloc()...                        */
#include <string.h>   /* strlen(), strncat, strcopy()...    */

char* strmerge(char* s1, char* s2) {
    char* merge = NULL;
    size_t len_s1 = strlen(s1);
    size_t len_s2 = strlen(s2);
    assert(s1 != NULL && s2 != NULL);
    merge = calloc(len_s1 + len_s2 + 1, sizeof(char));
    strncpy(merge, s1, len_s1);
    merge = strncat(merge, s2, len_s2);
    assert(merge != NULL && strlen(merge) == strlen(s1) + strlen(s2));
    return merge;
}

char* str_concat(char* s1, const char* s2) {
    assert(s1 != NULL && s2 != NULL);

    size_t s1_len = strlen(s1);
    size_t s2_len = strlen(s2);

    s1 = realloc(s1, s1_len + s2_len + sizeof(char)); // El + sizeof(char) es para el '\0'

    s1 = strcat(s1, s2);

    return (s1);
}
