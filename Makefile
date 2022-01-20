CFLAGS = -Wall

all: treasure

treasure: main.o base64.o command.o
				gcc main.o base64.o command.o -lncurses -ltinfo -o treasure

main.o: main.c command.h

base64.o: base64.c base64.h

command.o: command.c base64.h command.h

.c.o:
	gcc -c $< $(CFLAGS)

.SUFFIXES: .c .o

clean:
				-rm *.o

distclean: clean
	-rm treasure

dist: distclean
			(tar zcvf treasure.tgz ./; mv ./treasure.tgz ../)

.PHONY: all clean distclean
