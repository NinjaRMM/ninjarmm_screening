CC=x86_64-w64-mingw32-g++
CFLAGS = -fPIC -Werror -Wno-format-truncation -Wall -Wextra -O2
LDFLAGS = -static-libgcc -static-libstdc++ 
RM = rm -f

TARGET_BIN = appstep3.exe
SRCS_LIB = step3.cpp 
OBJS_LIB = $(SRCS_LIB:.cpp=.o)

.PHONY: all
all: $(TARGET_BIN) 

$(TARGET_BIN):
	$(CC) $(CFLAGS) -c -o step3.o step3.cpp
	$(CC) $(LDFLAGS) -o $@ $^ step3.o

.o:%.cpp
	$(CC) $(CFLAGS) $< >$@

.PHONY: clean
clean:
	@rm -f *.exe $(TARGET_BIN) *.o
