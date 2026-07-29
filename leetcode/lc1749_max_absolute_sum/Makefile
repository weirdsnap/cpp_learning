CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: lc1749_test.out

lc1749_test.out: lc1749_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: lc1749_test.out
	./lc1749_test.out

clean:
	rm -f lc1749_test.out
