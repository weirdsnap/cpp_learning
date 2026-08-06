CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc3345_test.out

.PHONY: all clean test

all: $(TARGETS)

lc3345_test.out: lc3345_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc3345_test.out
	./lc3345_test.out

clean:
	rm -f $(TARGETS)
