# Compiler flags
CFLAGS := -w -O3 -m64 -D'main(a,b)=main(int argc, char** argv)' -I boolector/src/ -L boolector/build/lib/ -L boolector/deps/lingeling/ -L boolector/deps/btor2tools/build/

# Compile selfie.c into selfie executable
phantom: phantom.c sase.c
	$(CC) $(CFLAGS) $^ -o $@ -lboolector -lbtor2parser -llgl