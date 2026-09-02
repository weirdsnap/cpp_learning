CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc253_test.out

.PHONY: all clean test

all: $(TARGETS)

lc253_test.out: lc253_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc253_test.out
	./lc253_test.out

clean:
	rm -f $(TARGETS)
