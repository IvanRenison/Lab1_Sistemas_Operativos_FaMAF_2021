TARGET=mybash
CC=gcc
CPPFLAGS=`pkg-config --cflags glib-2.0`
OBLIGATORYFLAGS=-std=gnu11 -Wall -Wextra -Wbad-function-cast -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes -Wno-unused-parameter -g -pedantic
CFLAGS=$(OBLIGATORYFLAGS) -Werror -Werror=vla
LDFLAGS=`pkg-config --libs glib-2.0`

# Propagar entorno a make en tests/
export CC CPPFLAGS CFLAGS LDFLAGS

SOURCES=$(shell echo *.c | sed 's/test_main.c//g')
OBJECTS=$(SOURCES:.c=.o)
PRECOMPILED=parser.o lexer.o

# Agregar objects-arch a los directorios de busqueda de los .o precompilados
ARCHDIR=objects-$(shell uname -m)
vpath parser.o $(ARCHDIR)
vpath lexer.o $(ARCHDIR)

all: $(TARGET)

$(TARGET): $(OBJECTS) $(PRECOMPILED)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJECTS) .depend *~
	make -C tests clean

test: $(OBJECTS)
	make -C tests test

test-command: command.o
	make -C tests test-command

pre-personal: $(PRECOMPILED) test_main.o command.o strextra.o prompt.o builtin.o
	$(CC) $(OBLIGATORYFLAGS) -o $@ $^ $(LDFLAGS)
	echo "RUN ./a.out"

personal:
	make clean
	make pre-personal

memtest: $(OBJECTS)
	make -C tests memtest

.depend: $(SOURCES)
	$(CC) $(CPPFLAGS) -MM $^ > $@

-include .depend

.PHONY: clean all
