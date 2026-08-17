CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc307_test.out

.PHONY: all clean test

all: $(TARGETS)

lc307_test.out: lc307_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc307_test.out
	./lc307_test.out

clean:
	rm -f $(TARGETS)
