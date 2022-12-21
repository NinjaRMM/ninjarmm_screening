PRJ_NAME=step5

C_SRC=$(wildcard *.cpp)
H_SRC=$(wildcard *.hpp)

OBJS=$(C_SRC:.cpp=.o)
CC=g++

CC_FLAGS= -std=c++20 -c -W -Wall 

.PHONY: all clean

clean: 
	rm $(PRJ_NAME) $(OBJS)

all: $(PRJ_NAME)

$(PRJ_NAME): $(OBJS)	
	@$(CC) $(OBJS)  -o $@

%.o: %.cpp 
	@echo "Building $@ $<"
	@$(CC) $(CC_FLAGS) -c $< -o $@



