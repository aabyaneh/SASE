# Compiler flags
CFLAGS := -w -O3 -m64 -D'main(a,b)=main(int argc, char** argv)'

# Compile selfie.c into selfie executable
phantom: phantom.c sase.c
	$(CXX) $(CFLAGS) $^ -o $@ -lz3