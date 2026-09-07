OBJ := $(patsubst %.c, %.o, $(wildcard *.c))

# Build target
Steganography.exe : $(OBJ)
	gcc -o $@ $^

# Clean build artifacts
clean:
	rm *.exe *.o