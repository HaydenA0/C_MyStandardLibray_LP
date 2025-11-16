
CFLAGS = -Wall -Wextra -ggdb

main: *.c
	clang $(CFLAGS) *.c -o ./bin/main


debug: main
	gdb ./bin/main



run: main
	./bin/main
