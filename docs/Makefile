SRC := proposal.md
TARGET := proposal.pdf

all: $(TARGET)

$(TARGET): $(SRC)
	pandoc $(SRC) -o $(TARGET)

preview:
	zathura --fork --log-level=error $(TARGET)

watch:
	echo '$(SRC)' | entr -c make
