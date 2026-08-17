CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc1510_test.out

.PHONY: all clean test

all: $(TARGETS)

lc1510_test.out: lc1510_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc1510_test.out
	./lc1510_test.out

clean:
	rm -f $(TARGETS)
