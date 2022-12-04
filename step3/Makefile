include jobs/config.mk

BIN := main
OBJ += main.o # main.cpp

CPPFLAGS += $(IPATH) -ggdb -O0

all: $(OBJ)
	g++ $(OBJ) $(CPPFLAGS) -o $(BIN)

clean:
	rm -vf $(OBJ) $(BIN)
