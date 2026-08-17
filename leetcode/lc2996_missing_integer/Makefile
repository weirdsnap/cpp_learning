CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc2996_test.out

.PHONY: all clean test

all: $(TARGETS)

lc2996_test.out: lc2996_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc2996_test.out
	./lc2996_test.out

clean:
	rm -f $(TARGETS)
