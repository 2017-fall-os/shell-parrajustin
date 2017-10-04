all: toc

CFLAGS = -g -O2

toc: shell.o token.o
	cc -o myShell shell.o token.o
clean:
	rm -f  *.o myShell myShell.exe
run: toc
	./myShell
test: toc
	./testShell.sh ./myShell