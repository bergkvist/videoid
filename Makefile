CC := g++
OPENCV := opencv4
CFLAGS := -g -Wall $(shell pkg-config $(OPENCV) --cflags) -O3 -D HASH_SIZE=32 -D WINDOW_SIZE=120
LIBS := $(shell pkg-config $(OPENCV) --libs)
OBJECTS := $(patsubst src/%.cpp, build/%.o, $(wildcard src/*.cpp))
TARGET := bin/main.exe
#YTVIDS := M_KWGJw6R24 ZTHsrEG5jhA j4hnIAqyhGE 9RlBXx8Pf-8

all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p ./bin/ && $(CC) -o $(TARGET) $(LIBS) $(OBJECTS)

build/%.o: src/%.cpp
	mkdir -p ./build/ && $(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -r ./build

download-videos:
	youtube-dl -f 'bestvideo[height<=144][ext=mp4]' -o './videos/%(id)s.%(ext)s' $(YTVIDS)

run: $(TARGET)
	./$(TARGET) ZTHsrEG5jhA M_KWGJw6R24

run-watch: $(TARGET)
	echo "src/main.cpp" | entr -c -s 'make run && make plot'

plot: images/out32.csv
	python images/plot.py

plot-watch: images/out32.csv
	echo "images/plot.py" | entr -c -s 'make plot'

test: $(TARGET)
	python ./bin/run.py

test-verbose: $(TARGET)
	VERBOSE=1 python ./bin/run.py