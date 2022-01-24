CFLAGS = -Wall

all: treasure

treasure: main.o base64.o aes.o command.o viewText.o
				gcc main.o base64.o aes.o command.o viewText.o -lncursesw -ltinfo -o treasure

main.o: main.c command.h

base64.o: base64.c base64.h

aes.o: aes.c aes.h

command.o: command.c base64.h aes.h viewText.h command.h

viewText.o: viewText.c viewText.h

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
