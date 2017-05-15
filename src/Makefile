# pir-medl.git/src/Makefile
# File ID: c608ae98-38e6-11e7-8438-f74d993421b0

OBJS = medl-add.o
CFILES = medl-add.c
HFILES = medl-add.h
DEPS = $(HFILES) Makefile
CC = cc
LD = cc
CCFLAGS = -Wall -Wextra -Werror -O2 -c -g -ansi -pedantic
LDFLAGS = -Wall
EXEC = medl-add

$(EXEC): $(OBJS)
	$(LD) -o $(EXEC) $(LDFLAGS) $(OBJS)

medl-add.o: medl-add.c $(DEPS)
	$(CC) $(CCFLAGS) medl-add.c

.PHONY: clean
clean:
	rm -f $(EXEC) $(OBJS) *~ core core.$(EXEC) tags
	cd t && $(MAKE) clean

.PHONY: edit
edit: tags
	$(EDITOR) *.c *.h Makefile

tags: $(CFILES) $(HFILES)
	ctags $(CFILES) $(HFILES)

.PHONY: test
test:
	cd t && $(MAKE) test
