CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc3418_test.out

.PHONY: all clean test

all: $(TARGETS)

lc3418_test.out: lc3418_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc3418_test.out
	./lc3418_test.out

clean:
	rm -f $(TARGETS)
