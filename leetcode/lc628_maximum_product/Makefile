CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc628_test.out

.PHONY: all clean test

all: $(TARGETS)

lc628_test.out: lc628_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc628_test.out
	./lc628_test.out

clean:
	rm -f $(TARGETS)
