CC := g++

# Use OpenCV 4 is available, otherwise attempt using OpenCV 3
OPENCV_FLAGS := $(shell pkg-config opencv4 --cflags || pkg-config opencv --cflags || echo "")
OPENCV_LIBS  := $(shell pkg-config opencv4 --libs   || pkg-config opencv --libs   || echo "")

CFLAGS := -g -Wall $(OPENCV_FLAGS)-O3 -D HASH_SIZE=32 -D WINDOW_SIZE=120
LIBS := $(OPENCV_LIBS)
OBJECTS := $(patsubst src/%.cpp, build/%.o, $(wildcard src/*.cpp))
TARGET := bin/main.exe

# === Build commands ===
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p ./bin/
	$(CC) -fopenmp -o $(TARGET) $(LIBS) $(OBJECTS)

build/%.o: src/%.cpp
	@mkdir -p ./build/
	$(CC) -fopenmp $(CFLAGS) -c -o $@ $<

clean:
	@rm -rf ./build && rm -f ./$(TARGET)
	@echo "Build directory and binary executable successfully deleted"

# === Commands for quick testing (you won't have to enter any id's) === 
run: $(TARGET)
	./$(TARGET) ZTHsrEG5jhA M_KWGJw6R24

run-verbose: $(TARGET)
	VERBOSE=1 make run
	
test: $(TARGET)
	python ./bin/run.py

test-verbose: $(TARGET)
	VERBOSE=1 make test

# === The commands below have been used mainly for debugging/continous development purposes ===
run-watch: $(TARGET)
	echo "src/main.cpp" | entr -c -s 'make run && make plot'

plot: images/out32.csv
	python images/plot.py

plot-watch: images/out32.csv
	echo "images/plot.py" | entr -c -s 'make plot'

download-videos:
	youtube-dl -f 'bestvideo[height<=144][ext=mp4]' -o './videos/%(id)s.%(ext)s' $(YTVIDS)
