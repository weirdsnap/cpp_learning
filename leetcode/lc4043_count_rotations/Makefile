CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc4043_test.out

.PHONY: all clean test

all: $(TARGETS)

lc4043_test.out: lc4043_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc4043_test.out
	./lc4043_test.out

clean:
	rm -f $(TARGETS)
