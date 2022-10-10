CC = g++
CFLAGS = -ffunction-sections -fPIC -Werror -Wno-format-truncation -Wall -Wextra -O2 -I.
RM = rm -f

SRCS_LIB = step3.cpp 
OBJS_LIB = $(SRCS_LIB:.cpp=.o)

TARGET_BIN = appstep3

.PHONY: all
all: $(TARGET_BIN) 

$(TARGET_BIN):
	$(CC) $(CFLAGS) -c -o step3.o step3.cpp
	$(CC) -o $@ step3.o

.o:%.cpp
	$(CC) $(CFLAGS) $< >$@

.PHONY: clean
clean:
	@rm -f *.so $(TARGET_BIN) *.o
