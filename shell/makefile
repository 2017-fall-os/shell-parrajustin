all: toc

CFLAGS = -g -O2

toc: test.o mytoc.o
	cc -o test test.o mytoc.o
clean:
	rm -f  *.o test test.exe
run: toc
	./test