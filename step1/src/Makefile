HEADERS = 
OBJECTS = main.o 

default: my_test

%.o: %.c $(HEADERS)
	g++ -c $< -o $@

my_test: $(OBJECTS)
	g++ $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f my_test