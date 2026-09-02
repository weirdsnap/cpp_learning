CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc1165_test.out

.PHONY: all clean test

all: $(TARGETS)

lc1165_test.out: lc1165_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc1165_test.out
	./lc1165_test.out

clean:
	rm -f $(TARGETS)
