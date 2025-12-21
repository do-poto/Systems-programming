CC = gcc
file1 := bufferFIFOWr.c
file2 := bufferFIFOWr.o
file3 := bufferFIFOWr


make:
	$(CC) -c $(file1) -o $(file2)
	$(CC) $(file2) -o $(file3)
clean:
	rm $(file2) $(file3)
