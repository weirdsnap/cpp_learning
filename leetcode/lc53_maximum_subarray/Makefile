CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: lc53_test.out

lc53_test.out: lc53_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: lc53_test.out
	./lc53_test.out

clean:
	rm -f lc53_test.out
