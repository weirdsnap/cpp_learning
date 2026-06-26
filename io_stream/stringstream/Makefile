CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: stringstream.out stringstream_test.out

stringstream.out: stringstream.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

stringstream_test.out: stringstream_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: stringstream_test.out
	./stringstream_test.out

clean:
	rm -f stringstream.out stringstream_test.out
