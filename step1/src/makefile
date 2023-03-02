
CC = c++


clean:
	rm *.o

job.o: job.hpp job.cpp
	$(CC) -c job.cpp -o job.o -std=c++11

pilot.o: pilot.hpp job.hpp pilot.cpp
	$(CC) -c pilot.cpp -o pilot.o -std=c++11

programmer.o: programmer.hpp job.hpp programmer.cpp
	$(CC) -c programmer.cpp -o programmer.o -std=c++11

it: job.o pilot.o programmer.o
	$(CC) runit.cpp job.o pilot.o programmer.o -std=c++11
