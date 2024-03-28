# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c11

#Input file
INPUT = input.txt
OUTPUT = output.txt
REF = ref.txt
# Source file
SRCS = assignment3.c
# Output
TARGET = assignment3.out

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

convert_input: $(INPUT)
	@if [ -s $(INPUT) ] && file $(INPUT) | grep -q 'CRLF line terminators' ; then \
    { cat $(INPUT) | dos2unix; } | tee $(INPUT) >/dev/null; \
	fi
	@if [ -s $(INPUT) ] && tail -c1 $(INPUT) | grep -q . ; then \
    echo >> $(INPUT); \
    	tr -d '\r' < $(INPUT) > temp_file && mv temp_file $(INPUT); \
	fi

convert_output: $(OUTPUT)
	@if [ -s $(OUTPUT) ] && file $(OUTPUT) | grep -q 'CRLF line terminators' ; then \
    { cat $(OUTPUT) | dos2unix; } | tee $(OUTPUT) >/dev/null; \
	fi
	@if [ -s $(OUTPUT) ] && tail -c1 $(OUTPUT) | grep -q . ; then \
    echo >> $(OUTPUT); \
    	tr -d '\r' < $(OUTPUT) > temp_file && mv temp_file $(OUTPUT); \
	fi

check: $(OUTPUT) $(REF)
	@if [ -s $(REF) ] && file $(REF) | grep -q 'CRLF line terminators' ; then \
    { cat $(REF) | dos2unix; } | tee $(REF) >/dev/null; \
	fi
	@if [ -s $(REF) ] && tail -c1 $(REF) | grep -q . ; then \
    echo >> $(REF); \
    	tr -d '\r' < $(REF) > temp_file && mv temp_file $(REF); \
	fi
	@diff -q $(OUTPUT) $(REF) > /dev/null; \
	if [ $$? -eq 0 ]; then \
		echo "Pass"; \
	else \
		echo "Fail"; \
	fi

# Rule to run the program
run: $(TARGET)
	./$(TARGET) $(INPUT) $(OUTPUT)

# Rule to clean generated files
clean:
	rm -f $(TARGET)
