CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: lc152_test.out

lc152_test.out: lc152_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: lc152_test.out
	./lc152_test.out

clean:
	rm -f lc152_test.out
