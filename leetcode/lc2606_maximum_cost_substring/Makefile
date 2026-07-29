CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: lc2606_test.out

lc2606_test.out: lc2606_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: lc2606_test.out
	./lc2606_test.out

clean:
	rm -f lc2606_test.out
