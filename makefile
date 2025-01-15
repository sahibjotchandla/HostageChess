CC=clang
CFLAGS=-std=c99 -Wall -pedantic -fPIC
LIBRARY_NAME=libhclib.so
EXE=test1

all: $(LIBRARY_NAME) $(EXE)

hclib.o: hclib.c
	$(CC) $(CFLAGS) -c hclib.c -o hclib.o

$(LIBRARY_NAME): hclib.o
	$(CC) -shared -o $(LIBRARY_NAME) hclib.o

$(EXE): test1.c
	$(CC) $(CFLAGS) -L. -lhclib test1.c -o $(EXE)

clean:
	rm -f *.o $(LIBRARY_NAME) $(EXE)

# Run with LD_LIBRARY_PATH for Linux
run: all
	export LD_LIBRARY_PATH=`pwd` && ./$(EXE)
