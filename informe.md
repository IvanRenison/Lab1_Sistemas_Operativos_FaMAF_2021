# Alumnos
- Fuentes, Tiffany
- Renison, Iván
- Vispo, Valentina Solange

# Contenido
- [Comprensión](#Comprensión)
    - [Parser](#Parser)
    - [Syscall](#Syscall)
- [Extra](#Extra:-nuestra-forma-de-trabajar)

# ¿Cómo correrlo?

## Test
```bash
make
```

```bash
gcc test_main.c command.o strextra.o `pkg-config --cflags --libs glib`
```

```bash
./a.out
```

## Debuger
```bash
make
```

```bash
gcc test_main.c command.o strextra.o `pkg-config --cflags --libs glib`
```

```bash
valgrind ./a.out
```

# Comprensión
Explicaciones simples de lo que hemos ido comprendiendo de los conceptos que fuimos implementando

## Archivos .h

Decidimos invertir el orden de las descripciones de las funciones para que en los editores de texto al pasar el cursor por encima se logre visualizar el mismo y que no sea necesario entrar a *recordar* el código de esa función o ir directamente al archivo **.h**. Esto nos permite ahorrarnos tiempos.

## Valgrind: memory leaks de la librería
**Primer intento de solucionarlo**

```bash
G_SLICE=always-malloc
```

Reference:
- https://stackoverflow.com/questions/19871083/how-to-correct-the-valgrind-errors

**Segundo intento de solucionarlo**

```bash
valgrind --suppressions="/usr/share/glib-2.0/valgrind/glib.supp" ./a.out
```

Referencias:
> If you use the glib.supp suppression file installed by GLib in /usr/share/glib-2.0/valgrind/glib.supp, they should be suppressed. If any of them aren’t, please file a bug against GLib."
- https://stackoverflow.com/questions/12389092/does-glib2-actually-leak-memory-with-always-malloc
- https://www.valgrind.org/docs/manual/manual-core.html#manual-core.suppress

**Tercer intento de solucionarlo**

```bash
valgrind --suppressions=/usr/share/glib-2.0/valgrind/glib.supp --show-reachable=no ./a.out
```

## Command
### Test
Al utilizar el `test_main.c` para probar las funciones del TAD, los strings que se utilicen para pasar como argumentos deben ser creados asignando memoria dinámica, ya que en la función `scommand_destroy` se libera la memoria ocupada por los mismos y si se declara el puntero de manera estática el mismo se cargará en el **heap** y va a causar un **error** cuando se **intente liberar la memoria**, se recomienda utilizar directamente las funciones `strdup()` y `strndup()` para la creación de strings.

Referencia:
> https://www.geeksforgeeks.org/strdup-strdndup-functions-c/

> https://stackoverflow.com/questions/20297524/c-free-invalid-pointer

## Builtin
Modificamos el archivo `builtin.h` y `builtin.c` porque:
1. 
2. En bash no se puede recibir varios argumentos.
3.
4.
5.
6.
7. 

NOTAS:
- `chdir()` ya reconoce si la llamada es desde home, sistema o el directorio actual por lo que se abstrae esta lógica en esta función.

## Syscall
`cd ~` desde el home

`cd /` desde el sistema

`cd ./` o `cd ` desde el directorio actual

### Sobre el funcionamiento de cd en el parser y su funcionamiento en Bash
En el GNU Bash, es lo mismo ejecutar `cd ~` y `cd `, ambas sirven para cambiar al directorio principal, en el caso de que se cree una carpeta o directorio llamado `~` al ejecutarse `cd ~` no se cambia a la carpeta/directorio creado, se cambia al directorio principal, en caso de querer referirse a una carpeta/directorio llamado `~` se debe ejecutar `cd '~'`, de igual forma para acceder a alguna carpeta o subdirectorio en el mismo, sebe ejecutar `cd '~'/archivos` por ejemplo, en caso de que tal carpeta se llame "archivos".
La implementación en este bash es similar, también, al igual que en el GNU Bash, en caso de que se cree una carpeta/directorio que tenga archivos adicionales ademas del `~`, por ejemplo `~archivos` no hace falta utilizar las comillas simples para cambiar a ese directorio, es decir se puede hacer `cd ~archivos`.
También es importante mencionar que en caso de que algun subdirectorio/carpeta se llame `~`, no es necesario utilizar las comillas simples al referirse al mismo, ya que solo se analiza el directorio principal para ver si es una llamada al directorio principal, o si es una llamada a un directorio llamado `~`.
Ejemplo: path = ~/Archivos/~
Se debe ejecutar `cd '~'/Archivos/~`, y no `cd '~'/Archivos/'~'`

## Parser


# Extra: nuestra forma de trabajar
> En el archivo [todo.md](todo.md) se encuentran las diferentes consignas y tareas que realizabamos (no siempre es commiteado).

> Comunicación: [Telegram](https://web.telegram.org/) y [Discord](https://discord.com/)