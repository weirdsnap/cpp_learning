CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: lc3014_test.out

lc3014_test.out: lc3014_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: lc3014_test.out
	./lc3014_test.out

clean:
	rm -f lc3014_test.out
