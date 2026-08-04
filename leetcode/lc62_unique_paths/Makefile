CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc62_test.out

.PHONY: all clean test

all: $(TARGETS)

lc62_test.out: lc62_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc62_test.out
	./lc62_test.out

clean:
	rm -f $(TARGETS)
