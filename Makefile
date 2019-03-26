CC := g++
CFLAGS := -g -Wall $(shell pkg-config opencv4 --cflags) -O3
LIBS := $(shell pkg-config opencv4 --libs)
TARGET := bin/main.exe
YTVIDS := M_KWGJw6R24 ZTHsrEG5jhA

all: $(TARGET)

$(TARGET): build/main.o
	mkdir -p ./bin/ && $(CC) $^ -o $(TARGET) $(LIBS)

build/main.o: src/main.cpp
	mkdir -p ./build/ && $(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -r ./build ./bin

download-videos:
	youtube-dl -f 'bestvideo[height<=144][ext=mp4]' -o './videos/%(id)s.%(ext)s' $(YTVIDS)

run: $(TARGET)
	./$(TARGET)

run-watch: $(TARGET)
	echo "src/main.cpp" | entr make run