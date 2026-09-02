CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc531_test.out

.PHONY: all clean test

all: $(TARGETS)

lc531_test.out: lc531_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc531_test.out
	./lc531_test.out

clean:
	rm -f $(TARGETS)
