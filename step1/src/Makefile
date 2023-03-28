HEADERS = 
OBJECTS = main.o 

default: step1.exe

%.o: %.c $(HEADERS)
	g++ -c $< -o $@

step1.exe: $(OBJECTS)
	g++ $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f step1.exe