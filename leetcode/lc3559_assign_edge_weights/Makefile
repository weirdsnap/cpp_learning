CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: lc3559_test.out

lc3559_test.out: lc3559_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: lc3559_test.out
	./lc3559_test.out

clean:
	rm -f lc3559_test.out
