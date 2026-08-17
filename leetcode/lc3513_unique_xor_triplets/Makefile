CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc3513_test.out

.PHONY: all clean test

all: $(TARGETS)

lc3513_test.out: lc3513_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc3513_test.out
	./lc3513_test.out

clean:
	rm -f $(TARGETS)
