CFLAGS = -Wall

all: escape

escape: main.o ossl.o
				gcc main.o ossl.o -lncurses -ltinfo -o escape

main.o: main.c ossl.h

ossl.o: ossl.c ossl.h

.c.o:
	gcc -c $< $(CFLAGS)

.SUFFIXES: .c .o

clean:
				-rm *.o

distclean: clean
	-rm escape

dist: distclean
			(tar zcvf escape.tgz ./; mv ./escape.tgz ../)

.PHONY: all clean distclean
