CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: lc2095_test.out

lc2095_test.out: lc2095_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: lc2095_test.out
	./lc2095_test.out

clean:
	rm -f lc2095_test.out
