all: main.o
	$(MAKE) -C lib
	$(MAKE) -C days

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o