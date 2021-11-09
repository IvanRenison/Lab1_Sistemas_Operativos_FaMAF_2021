# Corrección

* Grupo: 06
* Docente: Milagro Teruel
* Commit: 564154c
* Porcentaje: 100%

|                    | Concepto                                                          | Comentario                           | Estado |
|--------------------|-------------------------------------------------------------------|--------------------------------------|--------|
| Entrega & Informe  | En tiempo 13-09/21 23.59hs                                        |                                      | ok     |
|                    | Commit de todos los integrantes                                   |                                      | ok     |
|                    | Commit frecuentes                                                 |                                      | ok     |
|                    | No Entregaron código objeto y/o ejecutables                       |                                      | ok     |
|                    | Nivel de legibilidad, formato y estructura del informe            |                                      | ok     |
|                    | Detallan decisiones de implementación y compilación en el informe |                                      | ok     |
|                    |                                                                   |                                      |        |
| Calidad del Codigo | Comentarios explicativos en el código                             |                                      | ok     |
|                    | Uniformidad Idiomática                                            |                                      | ok     |
|                    | Consistencia en los TABs, espacios, identacion                    |                                      | ok     |
|                    | Líneas de codigo "cortas" y bloques de codigo "chicos"            |                                      | ok     |
|                    | Nombres de variables informativos y utilizacion de constantes     |                                      | ok     |
|                    | Inicialización de todas las variables                             |                                      | ok     |
|                    | Memory leaks                                                      |                                      | ok     |
|                    | Codigo redundante                                                 |                                      | ok     |
|                    | Sencillez y legibilidad                                           |                                      | ok     |
|                    | Adaptabilidad al cambio de símbolos para los operadores del bash  |                                      | ok     |
|                    | Adaptabilidad al cambio prompt                                    |                                      | ok     |
|                    | Adaptabilidad a nuevos comandos internos                          |                                      | ok     |
|                    | Adaptabilidad a manejar múltiples comandos separados por ; & \|   |                                      | ok     |
|                    | Modularizacion y encapsulamiento                                  |                                      | ok     |
|                    |                                                                   |                                      |        |
| Puntos estrellas   | Generalizacion a pipelines de tamaño arbitrario                   |                                      | Si     |
|                    | Implementacion del operador &&                                    |                                      | Si     |
|                    | Mejora del prompt                                                 |                                      | Si     |
|                    | Extension de la gramatica de mybash                               |                                      | Si     |
|                    | Cualquier otra mejora propuesta por el grupo                      |                                      |        |
|                    |                                                                   |                                      |        |
| Funcionalidad      | comando simple                                                    | ls                                   | ok     |
|                    | comando simple con 1 argumento                                    | ls -l                                | ok     |
|                    | comando simple con redireccion de entrada                         | sort < mybash.c                      | ok     |
|                    | comando simple con redireccion de salida                          | ls > salida.txt                      | ok     |
|                    | comando simple con redireccion de entrada y salida                | sort < mybash.c > salida.txt         | ok     |
|                    | comando simple en background                                      | xeyes &                              | ok     |
|                    | comando simple con 1 argumento en background                      | xeyes -fg blue &                     | ok     |
|                    | pipe simple                                                       | ls \| sort                           | ok     |
|                    | pipe simple con redirecion de salida                              | ls > salida_ls \| sort               | ok     |
|                    | pipe simple con doble redirecion de salida                        | ls > salida_ls \| sort > salida_sort | ok     |
|                    | pipe espera finalizacion de todos los hijos                       | sleep 10 \| echo "HOLA"              | ok     |
|                    | comando interno exit                                              |                                      | ok     |
|                    | comando interno cd                                                |                                      | ok     |
|                    | Entrada por stdin                                                 | echo -en "ls\nexit\n" \| ./mybash    | ok     |
|                    | CTRL-D para salir                                                 |                                      | ok     |
|                    | Robustez del bash                                                 | estecomandonoexiste -v               | ok     |
|                    |                                                                   | ls \|                                | ok     |
|                    |                                                                   | ls >                                 | ok     |
|                    |                                                                   | ls & \|                              | ok     |