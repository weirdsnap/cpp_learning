CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc146_test.out

.PHONY: all clean test

all: $(TARGETS)

lc146_test.out: lc146_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc146_test.out
	./lc146_test.out

clean:
	rm -f $(TARGETS)
