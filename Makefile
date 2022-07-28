all:
	gcc main.c linker.lds -o main.bin
	g++ checker.cpp -o checker.bin
	./checker.bin ./main.bin
	chmod +x /tmp/final.bin
	/tmp/final.bin
clean:
	rm -vf *.bin
