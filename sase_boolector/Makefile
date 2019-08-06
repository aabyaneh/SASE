CFLAGS := -w -O3 -m64 -D'main(a,b)=main(int argc, char** argv)'
# -I path_to_boolector/src/ -L path_to_boolector/build/lib/ -L path_to_boolector/deps/lingeling/ -L path_to_boolector/deps/btor2tools/build/

# Compile selfie.c into selfie executable
phantom: phantom.c sase.c
	$(CC) $(CFLAGS) $^ -o $@ -lboolector -lbtor2parser -llgl