CFLAGS = -g -Wall -Wextra -pedantic
OBJECTS := advent.o hashmap.o regex.o realloc_arr.o

all: $(OBJECTS)

$(OBJECTS): %.o: %.c

clean:
	rm $(OBJECTS)