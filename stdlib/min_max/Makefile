CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: min_max_test.out

min_max_test.out: min_max_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: min_max_test.out
	./min_max_test.out

clean:
	rm -f min_max_test.out
