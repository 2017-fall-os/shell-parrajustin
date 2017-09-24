all: toc

CFLAGS = -g -O2

toc: shell.o token.o
	cc -o test shell.o token.o
clean:
	rm -f  *.o test test.exe
run: toc
	./test