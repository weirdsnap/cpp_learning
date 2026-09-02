CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc3568_test.out

.PHONY: all clean test

all: $(TARGETS)

lc3568_test.out: lc3568_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc3568_test.out
	./lc3568_test.out

clean:
	rm -f $(TARGETS)
