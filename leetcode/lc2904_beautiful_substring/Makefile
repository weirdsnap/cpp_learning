CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc2904_test.out

.PHONY: all clean test

all: $(TARGETS)

lc2904_test.out: lc2904_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc2904_test.out
	./lc2904_test.out

clean:
	rm -f $(TARGETS)
