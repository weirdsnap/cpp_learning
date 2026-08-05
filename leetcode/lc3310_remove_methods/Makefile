CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc3310_test.out

.PHONY: all clean test

all: $(TARGETS)

lc3310_test.out: lc3310_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc3310_test.out
	./lc3310_test.out

clean:
	rm -f $(TARGETS)
