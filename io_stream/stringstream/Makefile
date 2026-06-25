CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: stringstream stringstream_test

stringstream: stringstream.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

stringstream_test: stringstream_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: stringstream_test
	./stringstream_test

clean:
	rm -f stringstream stringstream_test
