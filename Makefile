FLAGS = -Wall -Wno-unused-result

DEPS = cell.h matrix.h
OBJ = cell.c matrix.c main.c

%.o: %.c $(DEPS)
	gcc -g -c -o $@ $< $(FLAGS)

all: $(OBJ)
	gcc -g -o main $(OBJ) $(FLAGS)

run: 
	./main

clean:
	rm -f main *.o
	rm -rf matrix.bin

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./main
	