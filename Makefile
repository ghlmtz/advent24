SUBDIRS := src
OBJS = src/lib/advent.o src/lib/hashmap.o src/main.o src/days/1/solution.o \
	src/days/2/solution.o src/days/3/solution.o src/days/4/solution.o src/days/5/solution.o \
	src/days/6/solution.o src/days/7/solution.o src/days/8/solution.o src/days/9/solution.o \
	src/days/10/solution.o src/days/11/solution.o src/days/12/solution.o src/days/13/solution.o \
	src/days/14/solution.o src/days/15/solution.o src/days/16/solution.o src/days/17/solution.o \
	src/days/18/solution.o src/days/19/solution.o src/days/20/solution.o src/days/21/solution.o \
	src/days/22/solution.o src/days/23/solution.o src/days/24/solution.o src/days/25/solution.o \
	src/lib/dyn_arr.o src/lib/regex.o src/lib/xy_pos.o
export CFLAGS = -g -Wall -Wextra -pedantic

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)
	$(CC) $(LDFLAGS) -o advent $(OBJS)

$(SUBDIRS):
	$(MAKE) -C $@

src: src/lib

clean:
	rm -f $(OBJS)

