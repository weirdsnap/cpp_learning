CXX = g++
CXXFLAGS := -std=c++23 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc2213_test.out

.PHONY: all clean test

all: $(TARGETS)

lc2213_test.out: lc2213_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc2213_test.out
	./lc2213_test.out

clean:
	rm -f $(TARGETS)
