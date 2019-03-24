CC := g++
CFLAGS := -g -Wall
TARGET := bin/main.exe

all: $(TARGET)

$(TARGET): build/main.o
	mkdir -p ./bin/
	$(CC) $^ -o $(TARGET)

build/main.o: src/main.cpp
	mkdir -p ./build/
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -r ./build ./bin

run: $(TARGET)
	./$(TARGET)