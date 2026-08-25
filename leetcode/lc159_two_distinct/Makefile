CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc159_test.out

.PHONY: all clean test

all: $(TARGETS)

lc159_test.out: lc159_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc159_test.out
	./lc159_test.out

clean:
	rm -f $(TARGETS)
