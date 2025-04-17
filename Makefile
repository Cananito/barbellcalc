###
# CLI
###

CFLAGS=-Wall -Wextra -Werror

run: barbellcalc
	./barbellcalc

barbellcalc: calc.o main.o
	$(CC) $(CFLAGS) calc.o main.o -o barbellcalc

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

calc.o: calc.c
	$(CC) $(CFLAGS) -c calc.c -o calc.o

clean:
	rm -f *.o
	rm -f barbellcalc
	rm -rf barbellcalc.dSYM/


###
# Webapp
###

build-wasm: calc.c
	clang --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined -o calc.wasm calc.c

build-wasm-debug: calc.c
	clang -O0 -g --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined -o calc.wasm calc.

server:
	python3 -m http.server 9999

