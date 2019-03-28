CC := g++
CFLAGS := -g -Wall $(shell pkg-config opencv4 --cflags) -O3 -D HASH_SIZE=32
LIBS := $(shell pkg-config opencv4 --libs)
OBJECTS := $(patsubst src/%.cpp, build/%.o, $(wildcard src/*.cpp))
TARGET := bin/main.exe
YTVIDS := M_KWGJw6R24 ZTHsrEG5jhA

all: $(TARGET)

$(TARGET): $(OBJECTS) src/config.h
	mkdir -p ./bin/ && $(CC) -o $(TARGET) $(LIBS) $(OBJECTS)

build/%.o: src/%.cpp
	mkdir -p ./build/ && $(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -r ./build ./bin

download-videos:
	youtube-dl -f 'bestvideo[height<=144][ext=mp4]' -o './videos/%(id)s.%(ext)s' $(YTVIDS)

run: $(TARGET)
	./$(TARGET)

run-watch: $(TARGET)
	echo "src/main.cpp" | entr -c -s 'make run && make plot'

plot: images/out8.csv
	python images/plot.py

plot-watch: images/out8.csv
	echo "images/plot.py" | entr -c -s 'make plot'