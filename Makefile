CFLAGS = -pthread
OBJECTS = wprint/wprint.c main.c draw/draw.c tSize/terminal_size.c list/list.c layers/leyers2.c input/input.c
make: $(OBJECTS)
	gcc -o a $(OBJECTS) $(CFLAGS)
clean:
	rm a
	rm a.exe
run: make
	./a
test: make
	gcc -o a $(OBJECTS) $(CFLAGS) -g
	gdb ./a
profile: make
	@echo "perf report to view"
	@sleep 2
	perf record ./a
a: $(OBJECTS)
	gcc -o a $(OBJECTS) $(CFLAGS)

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
input.o: input/input.c
	gcc -c input/input.c $(CFLAGS)
wprint.o: wprint.c
	gcc -c wprint.c $(CFLAGS)
