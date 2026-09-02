CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc760_test.out

.PHONY: all clean test

all: $(TARGETS)

lc760_test.out: lc760_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc760_test.out
	./lc760_test.out

clean:
	rm -f $(TARGETS)
