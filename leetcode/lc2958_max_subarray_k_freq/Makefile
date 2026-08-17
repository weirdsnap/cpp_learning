CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc2958_test.out

.PHONY: all clean test

all: $(TARGETS)

lc2958_test.out: lc2958_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc2958_test.out
	./lc2958_test.out

clean:
	rm -f $(TARGETS)
