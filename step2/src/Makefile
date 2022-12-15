PRJ_NAME=step

C_SRC=$(wildcard *.cpp)
H_SRC=$(wildcard *.hpp)

OBJ=$(C_SRC:.cpp=.o)
CC=g++

CC_FLAGS=-c -W -Wall

all: $(PRJ_NAME)

$(PRJ_NAME): $(OBJ)
	$(CC) -o $@ $^

%.o: %.cpp %.h
	$(CC) -o $@ $<$(CC_FLAGS)

