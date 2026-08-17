CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc624_test.out

.PHONY: all clean test

all: $(TARGETS)

lc624_test.out: lc624_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc624_test.out
	./lc624_test.out

clean:
	rm -f $(TARGETS)
