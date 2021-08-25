# Alumnos
- Fuentes, Tiffany
- Renison, Iván
- Vispo, Valentina Solange

# Contenido
- [Comprensión](#Comprensión)
    - [Parser](#Parser)
    - [Syscall](#Syscall)
- [Extra](#Extra:-nuestra-forma-de-trabajar)

# Comprensión
Explicaciones simples de lo que hemos ido comprendiendo de los conceptos que fuimos implementando

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

## Syscall

## Parser


# Extra: nuestra forma de trabajar
> En el archivo [todo.md](todo.md) se encuentran las diferentes consignas y tareas que realizabamos (no siempre es commiteado)