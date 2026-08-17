CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc3090_test.out

.PHONY: all clean test

all: $(TARGETS)

lc3090_test.out: lc3090_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc3090_test.out
	./lc3090_test.out

clean:
	rm -f $(TARGETS)
