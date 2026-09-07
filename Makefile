OBJ := $(patsubst %.c, %.o, $(wildcard *.c))

# Build target
Steganography: $(OBJ)
	gcc -o $@ $^

# Clean build artifacts
clean:
	rm *.o