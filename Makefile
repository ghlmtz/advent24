SUBDIRS := lib/src src
OBJS = lib/src/advent.o lib/src/hashmap.o src/main.o src/1/solution.o \
	src/2/solution.o src/3/solution.o src/4/solution.o src/5/solution.o \
	src/6/solution.o src/7/solution.o src/8/solution.o src/9/solution.o \
	src/10/solution.o src/11/solution.o src/12/solution.o src/13/solution.o \
	src/14/solution.o src/15/solution.o src/16/solution.o src/17/solution.o \
	src/18/solution.o src/19/solution.o src/20/solution.o src/21/solution.o \
	src/22/solution.o src/23/solution.o src/24/solution.o src/25/solution.o \
	lib/src/realloc_arr.o

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)
	$(CC) $(LDFLAGS) -o advent $(OBJS)

$(SUBDIRS):
	$(MAKE) -C $@

src: lib/src

clean:
	rm -f $(OBJS)

