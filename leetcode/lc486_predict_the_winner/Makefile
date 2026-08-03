CXX = g++
CXXFLAGS := -std=c++23 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc486_test.out

.PHONY: all clean test

all: $(TARGETS)

lc486_test.out: lc486_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc486_test.out
	./lc486_test.out

clean:
	rm -f $(TARGETS)
