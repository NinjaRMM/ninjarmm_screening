.PHONY: all test

CXX=g++
CXX_ARGS=--std=c++17

build: step3
run: step3
	./step3

step3: CodeChallenge.cpp
	$(CXX) $(CXX_ARGS) CodeChallenge.cpp -o $@

clean:
	rm step3
