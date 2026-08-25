CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc1686_test.out

.PHONY: all clean test

all: $(TARGETS)

lc1686_test.out: lc1686_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc1686_test.out
	./lc1686_test.out

clean:
	rm -f $(TARGETS)
