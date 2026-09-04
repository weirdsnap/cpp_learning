CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc484_test.out

.PHONY: all clean test

all: $(TARGETS)

lc484_test.out: lc484_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc484_test.out
	./lc484_test.out

clean:
	rm -f $(TARGETS)
