# generated binary name
BINARY=pizzatime

# compiler info: which compiler to use and flags
CC=mpicc
CCFLAGS=-std=c99 -Wall -Werror

# build rules: the actual build rules for the program. Do not call locally.
all: main.o
	mkdir -p ../bin/ && $(CC) $(CCFLAGS) main.o -o ../bin/$(BINARY)

main.o: main.c 
	$(CC) $(CCFLAGS) -c main.c
