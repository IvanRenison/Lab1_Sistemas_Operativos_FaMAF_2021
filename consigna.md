# Index
- [Consigna](#Consigna)
    - [Algunos consejos](#Algunos-consejos)
- [Sobre la entrega del lab](#Sobre-la-entrega-del-lab)
- [Puntos estrellas](#Puntos-estrellas)
    - [Extras nuestros](#Extas-nuestros)

---

**[INFORME](informe.md) | [CONSIGNA](consigna.md) | [To Do](todo.md)**

---
# Consigna
## Algunos consejos
- Antes que nada completar y probar la implementación del TAD scommand y pipeline que será la interfaz de comunicación entre los módulos propuestos.

- Dividir el trabajo entre los integrantes del grupo. Esto servirá para trabajar en paralelo y focalizar a cada uno en una tarea particular. Luego si cada uno hizo bien su trabajo, se requerirá una etapa final de integración que no hay que subestimar en el tiempo que puede tomar.

- Hay muchísimas cosas de comportamiento no especificado, por ejemplo, qué hacer con `ls>out.txt|wc<in.txt`, o definir si el padre es peraatodos los hijos de un pipe o solo al último. Aunque no es necesario definir todos estos detalles y hacer que nuestro **shell** se comporte de esta manera, podemos deducir el comportamiento del programa haciendo experimentos en la línea de comandos de nuestro *NIX favorito.

- Codificar rápidamente el lazo principal de entrada; parse; execute paratenerunabasesobrelacualircodificandoyprobandodemanerainteractiva.

- Tratar en lo posible de ir haciendo la integración de los módulos de manera incremental, afindenoencontrarsorpresasenlaetapafinaldeintegración.

- Testear la implementación de manera exhaustiva, sobre todo en cuanto a su robustez.Pensar siempre que el usuario puede ser además de un enemigo, un gran conocedor de los bugs típicos que puede tener un shell.


# Sobre la entrega del lab

El proyecto deberá:
1) Ejecutar comandos simples con sus correspondientes argumentos y redirecciones de entrada y salida.

2) Manejar pipelines de hasta dos comandos simples.

3) Preservar las buenas prácticas de programación ya adquieridas durante los cursos anteriores (algoritmos 1 y 2).

4) Escribir un pequeño informe que detalle:
- cómo compilar su código,
- listar los comandos que ustedes probaron,
- reportar sobre las decisiones de implementación que se tomaron durante el proceso de desarrollo de su bash y/o todo aquello que ustedes consideren relevante.

> La entrega se hará directamente mediante un commit en el sistema de control de revisiones (bitbucket) que les asigna la cátedra.

> **Deadline: Lunes 13/09/2021 hasta las 23:59 hs (ARG) !!!**

# Puntos Estrellas

Se pueden optativa menterealizar las siguientes mejoras a nuestro bash:
- Generalizar el comando pipeline “`|`” a una cantidad arbitraria de comandos simples:
```bash
scomand_1 | ... | scommand_n
```
- Implementar el comando secuencial “&&” entre comandos simples:
```bash
scomand_1 && ... && scommand_n
```
- [x] Imprimir un **prompt** con información relevante, por ejemplo, nombre del host, nombre de usuario y camino relativo.

- Implementar toda la generalidad para aceptar la gramática de *list* según la sección SHELLGRAMMAR de man bash. Por ejemplo, se podrá ejecutar `ls -l | wc ; ls & ps`. Para hacer esto habrá que pensar mejor las estructuras porque pipeline incorpora el indicador de 2do plano que debería estar en *list*.

- Cualquier otra mejora que ustedes consideren relevante.

---
## Extras nuestros

- Modificación del archivo `buidtin.c`

> Hola, en el módulo builtin las funciones que hay, operan con pipelines, sin embargo, son cosas que se aplican a un solo comando, por ejemplo, la función builtin_is_internal está declarada como bool builtin_is_internal(pipeline pipe);, sin embargo, lo que puede o no ser interno es un scommand. Hacer que builtin trabaje sobre pipelines haría que no sea claro el comportamiento de las funciones en algunos casos, y creo que generaría complicaciones para el módulo execute, en donde creo que va a ser necesario usar esas funciones para un solo scommand.
Entonces, quería preguntar, ¿podemos modificar un poco el módulo builtin (agregar y modificar algunas funciones) para hacer esas cosas? O hay algún buen motivo para no hacerlo?

> En realidad, para lograr una mejor abstracción, lo que puede ser un builtin es el pipeline completo. Si lo modificás como sugerís, estarías "cerrando la puerta" a poder ejecutar pipelines enteros como builtin. Fijate que ahora solo hay dos, el cd y el exit, pero en un bash más real tendrías muchos comandos builtin. Sin embargo, tu punto es super válido, porque tampoco estamos definiendo con detalle qué pasa si tenés un pipeline con más de un comando, y un builtin en el medio, lo cual también es bastante improlijo.

> Para este lab, podés asumir que si el primer comando de un pipeline es un builtin, entonces el pipeline tiene un único comando. O sea, que no vas a recibir cosas como cd lala | ls -l. O, si lo recibís, sólo vas a ejecutar el cd y listo.

> Hola grupo! Si quieren modificar el builtin, me encanta la idea, pero i) haganlo una vez que terminen el execute, sería como "punto estrella", ii) no les prometo que no les robemos el código para el año que viene si queda lindo :P

---

**[INFORME](informe.md) | [CONSIGNA](consigna.md) | [To Do](todo.md)**
