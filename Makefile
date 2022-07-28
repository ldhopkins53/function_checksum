all:
	gcc main.c linker.lds -o main.bin
	g++ checker.cpp -o checker.bin
clean:
	rm -vf *.bin
