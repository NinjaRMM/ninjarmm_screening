HEADERS=
CXXFLAGS+=-std=c++17 -Wall -pedantic -DDEBUG -ggdb3
LDFLAGS= -pthread -lpthread
SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	g++ -o step1 $^ ${LDFLAGS}

%.o: %.cpp
	g++ ${CXXFLAGS} -c $<

clean:
	rm -f *.o step1
