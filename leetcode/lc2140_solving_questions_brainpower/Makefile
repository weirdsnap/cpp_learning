CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: lc2140_test.out

lc2140_test.out: lc2140_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: lc2140_test.out
	./lc2140_test.out

clean:
	rm -f lc2140_test.out
