# Alumnos
- Fuentes, Tiffany
- Renison, Iván
- Vispo, Valentina Solange

# Contenido
- [¿Cómo correrlo?](#¿Cómo-correrlo?)
    - [Correr nuestros tests](#Correr-nuestros-tests)
    - [Debuger](#Debuger)
- [Comprensión](#Comprensión)
    - [Archivos .h](#Archivos-.h)
    - [Valgrind: memory leaks de la librería](#Valgrind:-memory-leaks-de-la-librería)
    - [Command](#Command)
        - [Funciones command](#Funciones-command)
    - [Builtin](#Builtin)
        - [Funciones builtin](#Funciones-builtin)
    - [Syscall](#Syscall)
        - [Sobre el funcionamiento de cd en el parser y su funcionamiento en Bash](#Sobre-el-funcionamiento-de-cd-en-el-parser-y-su-funcionamiento-en-Bash)
    - [Parser](#Parser)
- [Extra: nuestra-forma-de-trabajar](#Extra:-nuestra-forma-de-trabajar)

# ¿Cómo correrlo?


## Correr nuestros tests
Realizar el siguiente comando en el path del **skeleton2021**

- Forma 1:

> Este archivo se encarga de correr `test_main.c`
```bash
make personal
```

- Forma 2:

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

### Funciones command

### Scomands

```c
typedef struct scommand_s* scommand;
```

>  Nuevo `scommand`, sin comandos o argumentos y los redirectores vacíos
>> Returns: nuevo comando simple sin ninguna cadena y redirectores vacíos.
```c
scommand scommand_new(void);
```

> Destruye `self`.
```c
scommand scommand_destroy(scommand self);
```

**Modificadores**

> Agrega por detrás una cadena a la secuencia de cadenas.
```c
void scommand_push_back(scommand self, char* argument);
```

> Quita la cadena de adelante de la secuencia de cadenas. `self`: comando simple al cual sacarle la cadena del frente.
```c
void scommand_pop_front(scommand self);
```

> Define la redirección de entrada. `self`: comando simple al cual establecer la redirección de entrada. `filename`: cadena con el nombre del archivo de la redirección o NULL si no se quiere redirección. El TAD se apropia de la referencia.
```c
void scommand_set_redir_in(scommand self, char* filename);
```

> Define la redirección de salida. `self`: comando simple al cual establecer la redirección de salida. `filename`: cadena con el nombre del archivo de la redirección o NULL si no se quiere redirección. El TAD se apropia de la referencia.
```c
void scommand_set_redir_out(scommand self, char* filename);
```

**Proyectores**

> Indica si la secuencia de cadenas tiene longitud 0. `self`: comando simple a decidir si está vacío.
>> Returns: ¿Está vacío de cadenas el comando simple?
```c
bool scommand_is_empty(const scommand self);
```

> Da la longitud de la secuencia cadenas que contiene el comando simple. `self`: comando simple a medir.
>> Returns: largo del comando simple.
```c
unsigned int scommand_length(const scommand self);
```

> Toma la cadena de adelante de la secuencia de cadenas. `self`: comando simple al cual tomarle la cadena del frente.
>> Returns: cadena del frente. La cadena retornada sigue siendo propiedad del TAD (ósea que el llamador no debe modificarla ni liberarla). Si se necesita una cadena propria hay que copiarla.
```c
char* scommand_front(const scommand self);
```


> Elimina la cadena de adelante de `self`, y la devuelve. La cadena retornada es propiedad del llamador, y debe ser liberada con **free**.
```c
char* scommand_front_and_pop(scommand self);
```

> Retorna el n-esimo argumento de `self`, la cadena retornada sigue siendo propiedad del TAD.
```c
char* scommand_get_nth(scommand self, unsigned int n);
```

```c
char* scommand_get_redir_in(const scommand self);
```

```c
char* scommand_get_redir_out(const scommand self);
```

> Convierte todos los argumentos de self en un arreglo de arreglos, que termina
 * en NULL. Los argumentos son eliminados de self, de forma que self queda vacía
 * El arreglo devuelto, y todos los arreglos que contiene son propiedad del
 * llamador
```c
char** scommand_to_argv(scommand self);
```

>  Genera una representación del comando simple en un string (aka "serializar") `self`: comando simple a convertir.
>> Returns: un string con la representación del comando simple similar a lo que se escribe en un shell. El llamador es dueño del string resultante.
```c
char* scommand_to_string(const scommand self);
```

### Pipelines

```c
typedef struct pipeline_s* pipeline;
```

> Nuevo `pipeline`, sin comandos simples y establecido para que espere.
>> Returns: nuevo pipeline sin comandos simples y que espera.
```c
pipeline pipeline_new(void);
```

> Destruye `self`.
```c
pipeline pipeline_destroy(pipeline self);
```

**Modificadores**

> Agrega por detrás un comando simple a la secuencia.
>> `sc`: comando simple a agregar. El TAD se apropia del comando.
```c
void pipeline_push_back(pipeline self, scommand sc);
```

> Quita el comando simple de adelante de la secuencia. Destruye el comando extraído.
```c
void pipeline_pop_front(pipeline self);
```

> Define si el pipeline tiene que esperar o no.
```c
void pipeline_set_wait(pipeline self, const bool w);
```

> Indica si la secuencia de comandos simples tiene longitud 0.
>> Returns: ¿Está vacío de comandos simples el pipeline?
```c
bool pipeline_is_empty(const pipeline self);
```

> Da la longitud de la secuencia de comandos simples.
>> Returns: largo del pipeline.
```c
unsigned int pipeline_length(const pipeline self);
```

> Devuelve el comando simple de adelante de la secuencia.
>> Returns: comando simple del frente. El comando devuelto sigue siendo propiedad del TAD. El resultado no es un "const scommand" ya que el llamador puede hacer modificaciones en el comando, siempre y cuando no lo destruya.
```c
scommand pipeline_front(const pipeline self);
```

> Elimina el comando de adelante de self, y la devuelve.
>> El comando retornado es propiedad del llamador, y debe ser destruido.
```c
scommand pipeline_front_and_pop(pipeline self);
```

> Consulta si el pipeline tiene que esperar o no.
>> Returns: ¿Hay que esperar en el pipeline self?
```c
bool pipeline_get_wait(const pipeline self);
```

> Genera una representación del pipeline en una cadena (aka "serializar").
>> Returns: una cadena con la representación del pipeline similar a lo que se escribe en un shell. Debe destruirla el llamador.
```c
char* pipeline_to_string(const pipeline self);
```

### Test
Al utilizar el `test_main.c` para probar las funciones del TAD, los strings que se utilicen para pasar como argumentos deben ser creados asignando memoria dinámica, ya que en la función `scommand_destroy` se libera la memoria ocupada por los mismos y si se declara el puntero de manera estática el mismo se cargará en el **heap** y va a causar un **error** cuando se **intente liberar la memoria**, se recomienda utilizar directamente las funciones `strdup()` y `strndup()` para la creación de strings.

Referencia:
> https://www.geeksforgeeks.org/strdup-strdndup-functions-c/

> https://stackoverflow.com/questions/20297524/c-free-invalid-pointer

## Builtin
Modificamos el archivo `builtin.h` y `builtin.c` porque:
1. En bash no se puede recibir varios argumentos.
2.

NOTAS:
- `chdir()` ya reconoce si la llamada es desde home, sistema o el directorio actual por lo que se abstrae esta lógica en esta función.

---
### Funciones builtin

> Indica si un "exit"
```c
bool builtin_scommand_is_exit(const scommand cmd);
```

> Indica si el comando es un "cd"
```c
bool builtin_scommand_is_cd(const scommand cmd);
```

> Indica si un comando es interno
```c
bool builtin_scommand_is_internal(const scommand cmd);
```

> Indica si el pipeline tiene un solo comando y ese comando es interno
```c
bool builtin_scommand_is_single_internal(const pipeline pipe);
```

> Ejecuta un comando interno
```c
void builtin_scommand_exec(const scommand cmd);
```

> Ejecuta una pipeline con un único comando
```c
void builtin_single_pipeline_exec(const pipeline pipe);
```

---
## Syscall
`cd ~` desde el home

`cd /` desde el sistema

`cd ./` o `cd ` desde el directorio actual

### Sobre el funcionamiento de cd en el parser y su funcionamiento en Bash
En el GNU Bash, es lo mismo ejecutar `cd ~` y `cd `, ambas sirven para cambiar al directorio principal, en el caso de que se cree una carpeta o directorio llamado `~` al ejecutarse `cd ~` no se cambia a la carpeta/directorio creado, se cambia al directorio principal, en caso de querer referirse a una carpeta/directorio llamado `~` se debe ejecutar `cd '~'`, de igual forma para acceder a alguna carpeta o subdirectorio en el mismo, sebe ejecutar `cd '~'/archivos` por ejemplo, en caso de que tal carpeta se llame "archivos".
La implementación en este bash es similar, también, al igual que en el GNU Bash, en caso de que se cree una carpeta/directorio que tenga archivos adicionales ademas del `~`, por ejemplo `~archivos` no hace falta utilizar las comillas simples para cambiar a ese directorio, es decir se puede hacer `cd ~archivos`.
También es importante mencionar que en caso de que algun subdirectorio/carpeta se llame `~`, no es necesario utilizar las comillas simples al referirse al mismo, ya que solo se analiza el directorio principal para ver si es una llamada al directorio principal, o si es una llamada a un directorio llamado `~`.
Ejemplo: path = ~/Archivos/~
Se debe ejecutar `cd '~'/Archivos/~`, y no `cd '~'/Archivos/'~'`, en el GNU Bash cualquiera de los dos funciona, pero en nuestra implementación sólo se debe colocar `'~'` en el directorio si se refiere a un directorio/carpeta llamado `~`.

## Parser


# Extra: nuestra forma de trabajar
> En el archivo [todo.md](todo.md) se encuentran las diferentes consignas y tareas que realizabamos (no siempre es commiteado).

> Pair programming con la extensión de [Live Share](https://visualstudio.microsoft.com/services/live-share/) de Visual Studio Code

> Comunicación: [Telegram](https://web.telegram.org/) y [Discord](https://discord.com/)

## Estilo del código

Configuración:

**.clang-format**
```yaml
BasedOnStyle: LLVM

IndentWidth: 4
PointerAlignment: Left
ReflowComments: false
```

**.vscode/settings.json**
```json
{
    "C_Cpp.clang_format_style": "file",
    "files.associations": {
        "*.s": "arm",
        "stdio.h": "c",
        "functional": "c",
        "strextra.h": "c",
        "builtin.h": "c",
        "cstdio": "cpp",
        "fstream": "cpp",
        "glib.h": "c",
        "random": "c",
        "stdlib.h": "c",
        "assert.h": "c",
        "unistd.h": "c"
    }
}
```

Referencia:

- [ClangFormatStyleOptions](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)
