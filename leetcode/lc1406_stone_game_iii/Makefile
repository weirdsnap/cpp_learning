CXX = g++
CXXFLAGS := -std=c++23 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc1406_test.out

.PHONY: all clean test

all: $(TARGETS)

lc1406_test.out: lc1406_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc1406_test.out
	./lc1406_test.out

clean:
	rm -f $(TARGETS)
