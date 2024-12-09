SOURCES := $(wildcard */*.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))

all: $(OBJECTS)

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -I../lib -c -o $@ $<