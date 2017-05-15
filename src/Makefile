# pir-medl.git/src/Makefile
# File ID: c608ae98-38e6-11e7-8438-f74d993421b0

OBJS = medl.o sqlite3.o
CFILES = medl.c
HFILES = medl.h
DEPS = $(HFILES) Makefile
CC = cc
LD = cc
CCFLAGS = -Wall -Wextra -Werror -O0 -c -g -pedantic
LDFLAGS = -Wall
LIBS = -lpthread -ldl
EXEC = medl

$(EXEC): $(OBJS)
	$(LD) -o $(EXEC) $(LDFLAGS) $(OBJS) $(LIBS)

medl.o: medl.c $(DEPS)
	$(CC) $(CCFLAGS) medl.c

sqlite3.o: sqlite3.c
	$(CC) $(CCFLAGS) -DSQLITE_ENABLE_JSON1 sqlite3.c

.PHONY: clean
clean:
	rm -f $(EXEC) $(OBJS) *~ core core.$(EXEC) tags
	cd t && $(MAKE) clean

.PHONY: edit
edit: tags
	$(EDITOR) $$(git ls-files | grep -v -e ^sqlite3)

tags: $(CFILES) $(HFILES)
	ctags $(CFILES) $(HFILES)

.PHONY: test
test:
	cd t && $(MAKE) test
