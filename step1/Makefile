.PHONY: all test

CXX=g++
CXX_ARGS=--std=c++17

build: step1
run: step1
	./step1


step1: step1.cc
	$(CXX) $(CXX_ARGS) step1.cc -o $@

clean:
	rm stage1
