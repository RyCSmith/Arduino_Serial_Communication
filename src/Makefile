all: arduino

arduino: arduino.c
	clang -g -lpthread arduino.c -o arduino

clean:
	rm -rf *.o

clobber: clean
	rm -rf arduino