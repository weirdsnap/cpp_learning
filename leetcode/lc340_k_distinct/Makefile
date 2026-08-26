CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc340_test.out

.PHONY: all clean test

all: $(TARGETS)

lc340_test.out: lc340_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc340_test.out
	./lc340_test.out

clean:
	rm -f $(TARGETS)
