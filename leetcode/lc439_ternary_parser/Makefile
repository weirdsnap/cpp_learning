CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc439_test.out

.PHONY: all clean test

all: $(TARGETS)

lc439_test.out: lc439_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc439_test.out
	./lc439_test.out

clean:
	rm -f $(TARGETS)
