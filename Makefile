CFLAGS = -pthread
OBJECTS = wprint/wprint.c main.c draw.c tSize/terminal_size.c list/list.c layers/leyers2.c
make: $(OBJECTS)
	gcc -o a $(OBJECTS) $(CFLAGS)
clean:
	rm a
	rm a.exe
run: make
	./a


wprint.o: wprint.c
	gcc -c wprint.c $(CFLAGS)
main.o: main.c
	gcc -c main.c $(CFLAGS)
draw.o: draw.c
	gcc -c draw.c $(CFLAGS)

layers2.o: layers/leyers2.c
	gcc -c layers/leyers2.c $(CFLAGS)

tSize/terminal_size.o: tSize/terminal_size.c
	gcc -c tSize/terminal_size.c $(CFLAGS)

list/list.o: list/list.c
	gcc -c list/list.c $(CFLAGS)
