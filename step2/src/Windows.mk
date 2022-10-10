CC=x86_64-w64-mingw32-g++
CFLAGS = -fPIC -Werror -Wno-format-truncation -Wall -Wextra -O2
LDFLAGS = -static-libgcc -static-libstdc++ 
RM = rm -f

TARGET_BIN = appstep2.exe
SRCS_LIB = step2.cpp 
OBJS_LIB = $(SRCS_LIB:.cpp=.o)

.PHONY: all
all: $(TARGET_BIN) 

$(TARGET_BIN):
	$(CC) $(CFLAGS) -c -o step2.o step2.cpp
	$(CC) $(LDFLAGS) -o $@ $^ step2.o

.o:%.cpp
	$(CC) $(CFLAGS) $< >$@

.PHONY: clean
clean:
	@rm -f *.exe $(TARGET_BIN) *.o
