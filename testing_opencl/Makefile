CC = gcc
CFLAGS = -std=c99 -Wall # -I/opt/intel/opencl-sdk/include
LDFLAGS = -framework OpenCL

TARGET = prime_finder
SOURCES = main.c
KERNEL = kernel.c

$(TARGET): $(SOURCES) $(KERNEL)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(TARGET)