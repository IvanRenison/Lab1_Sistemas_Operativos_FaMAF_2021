# Consigna
## Algunos consejos
● Antes que nada completar y probar la implementación del TAD scommand y pipeline que será la interfaz de comunicación entre los módulos propuestos.

● Dividir el trabajo entre los integrantes del grupo. Esto servirá para trabajar en paralelo yfocalizar a cada uno en una tarea particular. Luego si cada uno hizo bien su trabajo, serequerirá una etapa final de integración que no hay que subestimar en el tiempo que puedetomar.

● Hay muchísimas cosas de comportamiento no especificado, por ejemplo, qué hacer con `ls>out.txt|wc<in.txt`, o definir si el padre es peraatodos los hijos de un pipe o solo al último. Aunque no es necesario definir todos estos detalles y hacer que nuestroshell se comporte de esta manera, podemos deducir el comportamiento del programa haciendo experimentos en la línea de comandos de nuestro *NIXfavorito.

● Codificar rápidamente el lazo principal de entrada; parse; execute paratenerunabasesobrelacualircodificandoyprobandodemanerainteractiva.

● Tratar en lo posible de ir haciendo la integración de los módulos de manera incremental, afindenoencontrarsorpresasenlaetapafinaldeintegración.

● Testear la implementación de manera exhaustiva, sobre todo en cuanto a su robustez.Pensar siempre que el usuario puede ser además de un enemigo, un gran conocedor de los bugs típicos que puede tener un shell.


## Sobre la entrega del lab

El proyecto deberá:
1) Ejecutar comandos simples con sus correspondientes argumentos y redireccionesde entrada y salida.
2) Manejarpipelinesde hasta doscomandos simples.
3) Preservar las buenas prácticas de programación ya adquieridasdurante los cursos anteriores (algoritmos 1 y 2).
4) Escribir un pequeño informe que detalle cómo compilar su código y listar los comandos que ustedes probaron. Además, reportar sobre las decisiones de implementación que se tomaron durante el proceso de desarrollo de su bash y/o todo aquello que ustedes consideren relevante.La entrega se hará directamente mediante un commit  en el sistema de control de revisiones(bitbucket) que les asigna la cátedra.

> **Deadline: Lunes 13/09/2021 hasta las 23:59 hs (ARG) !!!**

## Puntos Estrellas

Se pueden optativa menterealizar las siguientes mejoras a nuestro bash:
● Generalizar el comando pipeline “|” a una cantidad arbitraria de comandos simples:
```bash
scomand_1 | ... | scommand_n
```
● Implementar el comando secuencial “&&” entre comandos simples:
```bash
scomand_1 && ... && scommand_n
```
● Imprimir un prompt con información relevante, por ejemplo, nombre del host, nombre de usuario y camino relativo.
● Implementar toda la generalidad para aceptar la gramática de list según la sección SHELLGRAMMAR de man bash. Por ejemplo, se podrá ejecutar ls -l | wc ; ls & ps. Para hacer estohabrá que pensar mejor las estructuras porque pipeline incorpora el indicador de 2do planoquedeberíaestarenlist.
● Cualquier otra mejora que ustedes consideren relevante.
