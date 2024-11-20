CFLAGS = -g -I../lib/src -Wall -Wextra -pedantic
SOURCES := main.c $(wildcard */*.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))

all: $(OBJECTS)

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<