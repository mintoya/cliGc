CFLAGS = -pthread
OBJECTS = main.c tSize/terminal_size.c list/list.c

# The default target is to build the executable 'a'
make: $(OBJECTS)
	gcc -o a $(OBJECTS) $(CFLAGS)

# To run the program
run: make
	./a

# Rules for compiling source files
main.o: main.c
	gcc -c main.c $(CFLAGS)

tSize/terminal_size.o: tSize/terminal_size.c
	gcc -c tSize/terminal_size.c $(CFLAGS)

list/list.o: list/list.c
	gcc -c list/list.c $(CFLAGS)
