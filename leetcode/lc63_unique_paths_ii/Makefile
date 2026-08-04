CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc63_test.out

.PHONY: all clean test

all: $(TARGETS)

lc63_test.out: lc63_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc63_test.out
	./lc63_test.out

clean:
	rm -f $(TARGETS)
