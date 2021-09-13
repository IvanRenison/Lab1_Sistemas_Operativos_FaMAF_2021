# Alumnos

- Fuentes, Tiffany
- Renison, Iván
- Vispo, Valentina Solange

---

**[README](README.md) | [CONSIGNA](consigna.md) | [To Do](todo.md)**

---

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
gcc test_main.c command.o strextra.o `pkg-config --cflags --libs glib-2.0`
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
> 
> > Returns: nuevo comando simple sin ninguna cadena y redirectores vacíos.

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
> 
> > Returns: ¿Está vacío de cadenas el comando simple?

```c
bool scommand_is_empty(const scommand self);
```

> Da la longitud de la secuencia cadenas que contiene el comando simple. `self`: comando simple a medir.
> 
> > Returns: largo del comando simple.

```c
unsigned int scommand_length(const scommand self);
```

> Toma la cadena de adelante de la secuencia de cadenas. `self`: comando simple al cual tomarle la cadena del frente.
> 
> > Returns: cadena del frente. La cadena retornada sigue siendo propiedad del TAD (ósea que el llamador no debe modificarla ni liberarla). Si se necesita una cadena propria hay que copiarla.

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
> 
> > Returns: un string con la representación del comando simple similar a lo que se escribe en un shell. El llamador es dueño del string resultante.

```c
char* scommand_to_string(const scommand self);
```

### Pipelines

```c
typedef struct pipeline_s* pipeline;
```

> Nuevo `pipeline`, sin comandos simples y establecido para que espere.
> 
> > Returns: nuevo pipeline sin comandos simples y que espera.

```c
pipeline pipeline_new(void);
```

> Destruye `self`.

```c
pipeline pipeline_destroy(pipeline self);
```

**Modificadores**

> Agrega por detrás un comando simple a la secuencia.
> 
> > `sc`: comando simple a agregar. El TAD se apropia del comando.

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
> 
> > Returns: ¿Está vacío de comandos simples el pipeline?

```c
bool pipeline_is_empty(const pipeline self);
```

> Da la longitud de la secuencia de comandos simples.
> 
> > Returns: largo del pipeline.

```c
unsigned int pipeline_length(const pipeline self);
```

> Devuelve el comando simple de adelante de la secuencia.
> 
> > Returns: comando simple del frente. El comando devuelto sigue siendo propiedad del TAD. El resultado no es un "const scommand" ya que el llamador puede hacer modificaciones en el comando, siempre y cuando no lo destruya.

```c
scommand pipeline_front(const pipeline self);
```

> Elimina el comando de adelante de self, y la devuelve.
> 
> > El comando retornado es propiedad del llamador, y debe ser destruido.

```c
scommand pipeline_front_and_pop(pipeline self);
```

> Consulta si el pipeline tiene que esperar o no.
> 
> > Returns: ¿Hay que esperar en el pipeline self?

```c
bool pipeline_get_wait(const pipeline self);
```

> Genera una representación del pipeline en una cadena (aka "serializar").
> 
> > Returns: una cadena con la representación del pipeline similar a lo que se escribe en un shell. Debe destruirla el llamador.

```c
char* pipeline_to_string(const pipeline self);
```

### Test

Al utilizar el `test_main.c` para probar las funciones del TAD, los strings que se utilicen para pasar como argumentos deben ser creados asignando memoria dinámica, ya que en la función `scommand_destroy` se libera la memoria ocupada por los mismos y si se declara el puntero de manera estática el mismo se cargará en el **heap** y va a causar un **error** cuando se **intente liberar la memoria**, se recomienda utilizar directamente las funciones `strdup()` y `strndup()` para la creación de strings.

Referencia:

> https://www.geeksforgeeks.org/strdup-strdndup-functions-c/

> https://stackoverflow.com/questions/20297524/c-free-invalid-pointer

## Builtin

    El módulo `builtin`, en el esqueleto venía con una función que decidía si un `pipeline` es interno, otra función que ejecutaba un `pipeline` de comandos internos y ademas, para cada comando interno, una función que decidía si un `pipeline` era ese comando interno.

    Nosotros decidimos cambiar eso, ya que nos parecio que todas esas cosas eran cosas que aplicaban a un comando simple, y no a un pipeline, por lo cuál estructuramos el módulo de la siguete manera:

• Por cada comando interno, una función de la forma `builtin_scommand_is_` que toma un `scommand` y decide si es ese comando interno.

• Por cada comando interno, una función que toma un `scommand`, lo ejecuta y retorna (es muy importante que retorne, ya que los comandos internos a veces son ejecutados en el hilo principal del programa). Estás son lad funciones de la forma `builtin_run_`, y no están en el `.h`.

• El resto de las funciones son:

```c
bool builtin_scommand_is_internal(const scommand cmd);
bool builtin_scommand_is_single_internal(const pipeline pipe);
void builtin_scommand_exec(const scommand cmd);
void builtin_single_pipeline_exec(const pipeline pipe);
```

Que hacen lo que su nombre indica (en el `builtin.h` está mas detallado).

    Como se puede ver, no hay ninguna función que tome un `pipeline` de comandos internos y lo ejecute, o algo similar. Esto se debe a que los `pipeline` de varios comandos se ejecutan de la misma manera, tanto si tienen comandos internos, externos o ambas cosas mezcladas.

    Una consecuencia de eso, es que los comandos internos, cuando están en un `pipeline` de varios comandos se ejecutan en un hijo, y por ende, los que modifican el estado de la terminal (como por ejemplo `cd` y `exit`) no tienen efecto (en `bash` tambien es así).

### Syscall

`cd ~` desde el home

`cd /` desde el sistema

`cd ./` o `cd ` desde el directorio actual

### Sobre el funcionamiento de cd en el parser y su funcionamiento en Bash

En el GNU Bash, es lo mismo ejecutar `cd ~` y `cd `, ambas sirven para cambiar al directorio principal, en el caso de que se cree una carpeta o directorio llamado `~` al ejecutarse `cd ~` no se cambia a la carpeta/directorio creado, se cambia al directorio principal, en caso de querer referirse a una carpeta/directorio llamado `~` se debe ejecutar `cd '~'`, de igual forma para acceder a alguna carpeta o subdirectorio en el mismo, sebe ejecutar `cd '~'/archivos` por ejemplo, en caso de que tal carpeta se llame "archivos".
La implementación en este bash es similar, también, al igual que en el GNU Bash, en caso de que se cree una carpeta/directorio que tenga archivos adicionales ademas del `~`, por ejemplo `~archivos` no hace falta utilizar las comillas simples para cambiar a ese directorio, es decir se puede hacer `cd ~archivos`.
También es importante mencionar que en caso de que algun subdirectorio/carpeta se llame `~`, no es necesario utilizar las comillas simples al referirse al mismo, ya que solo se analiza el directorio principal para ver si es una llamada al directorio principal, o si es una llamada a un directorio llamado `~`.
Ejemplo: path = ~/Archivos/~
Se debe ejecutar `cd '~'/Archivos/~`, y no `cd '~'/Archivos/'~'`, en el GNU Bash cualquiera de los dos funciona, pero en nuestra implementación sólo se debe colocar `'~'` en el directorio si se refiere a un directorio/carpeta llamado `~`.

## Execute

    El módulo `execute` cuenta con una sola función en el `.h` que es `execute_pipeline`, y que se encarga de ejecutar un pipeline. En el `.c` hay una gran cantidad de funciones auxiliares que se llaman unas a otras.

    Cada función tiene arriba una descripción de que hace, y las pre y pos condiciones que tiene.

    A continuación hay un diagrama de como son las llamadas entre las funciones, en el cuál se puede ver cuando se hacen llamadas normales, y cuando se hacen llamadas haciendo primero un `fork` y llamando en el hijo.

<img title="" src="Imagenes informe/Diagrama execute.svg" alt="Diagrama execute.svg" width="868">

    Hay varias cosas que pueden resultar un poco extrañas sobre como están las llamadas. El motivo es para evitar que queden procesos zombies.

### Los procesos zombies:

    Cuando los procesos hijos terminan no son borrados por completo de la memoria de forma automatica, si no se se guarda cierta el información sobre la terminación del proceso para que el padre pueda acceder a esa información, y recien cuando accede esa información se borra. En C, con la función `wait` se puede esperar a que algún proceso hijo termine y obtener la información.

    Para ejecutar los pipelines la información de como termino no es necesaria para nada, y es necesario esperar a que el proceso hijo termine solo cuando el `pipeline` está seeteado para esperar.

    Si la única diferencia que se hace entre cuando esta para que espere, y cuando no, es hacer o no llamadas a `wait` después de crear los procesos hijos, lo que pasa es que cuando está seetado para que no espere los hijos terminan y quedan como procesos zombies.

    Para solucionar eso, lo primero que se nos había ocurrido fue hacer una función que terminara todos los hijos zombies existentes (usando `waitpid`), y a esa función llamarla en cada siclo de `while` en la función `main`. Sin embargo, esa solución no es muy buena, porque solo se eliminan los zombies cuando se presiona enter en el `mybash`, y por ende pueden quedar un tiempo los procesos zombies.

    Luego encontramos que cuando un proceso termina, sos procesos hijos pasan a ser hijos de `init` y no se convierten en zombies al terminar, por lo cuál, hicimos que al ejecutar un `pipeline`, si está seeteado para que no espere, lo que se hace es crear un hijo que sea el encargado de crear todos los hijos (nietos del original), y a ese hijo, hacer que termine con `exit` cuando termina de crear todos los hijos. De esa forma, desde el proceso principal solo se espera a ese hijo, y cuando ese hijo termina, todos sus hijos (nietos del original) pasan a ser hijos de `init`.

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

---

**[README](README.md) | [CONSIGNA](consigna.md) | [To Do](todo.md)**
