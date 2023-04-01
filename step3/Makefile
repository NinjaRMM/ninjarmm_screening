HEADERS = 
OBJECTS = CodeChallenge.o

default: step3.exe

%.o: %.c $(HEADERS)
	g++ -c $< -o $@

step3.exe: $(OBJECTS)
	g++ $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f step3.exe