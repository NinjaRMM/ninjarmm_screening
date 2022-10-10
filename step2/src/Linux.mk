CC = g++
CFLAGS = -ffunction-sections -fPIC -Werror -Wno-format-truncation -Wall -Wextra -O2 -I.
RM = rm -f

SRCS_LIB = step2.cpp 
OBJS_LIB = $(SRCS_LIB:.cpp=.o)

TARGET_BIN = appstep2

.PHONY: all
all: $(TARGET_BIN) 

$(TARGET_BIN):
	$(CC) $(CFLAGS) -c -o step2.o step2.cpp
	$(CC) -o $@ step2.o

.o:%.cpp
	$(CC) $(CFLAGS) $< >$@

.PHONY: clean
clean:
	@rm -f *.so $(TARGET_BIN) *.o
