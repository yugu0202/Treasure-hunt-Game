CFLAGS = -Wall

all: treasure

treasure: main.o base64.o aes.o command.o viewText.o fileSystem.o
				gcc main.o base64.o aes.o command.o viewText.o fileSystem.o -lncursesw -ltinfo -o treasure

main.o: main.c fileSystem.h command.h

base64.o: base64.c base64.h

aes.o: aes.c aes.h

command.o: command.c base64.h aes.h viewText.h fileSystem.h command.h

viewText.o: viewText.c viewText.h

fileSystem.o: fileSystem.c fileSystem.h

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
