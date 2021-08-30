#ifndef _STRETRA_H_
#define _STRETRA_H_

/*
 * Concatena las cadenas en s1 y s2 devolviendo nueva memoria (debe ser
 * liberada por el llamador con free())
 *
 * USAGE:
 *
 * merge = strmerge(s1, s2);
 *
 * REQUIRES:
 *     s1 != NULL &&  s2 != NULL
 *
 * ENSURES:
 *     merge != NULL && strlen(merge) == strlen(s1) + strlen(s2)
 *
 */
char * strmerge(char *s1, char *s2);

/*
 * Concatena 2 cadenas re-allocando la primera para agregar el espacio necesario.
 * s1 tiene que ser una cadena dinamico (declarada con malloc), pero s2
 * puede ser estático.
 * Notar que es distinto de strmerge, ya que strmerge crea una nueva cadena
 * en lugar de re-allocar la anterior
 *
 * USAGE: s1 = str_concat(s1, s2);
 *        s1 = str_concat(s1, "string");
 *
 * REQUIRES: s1 != NULL && s2 != NULL
 *
 */
char * str_concat(char * s1, const char * s2);

#endif
