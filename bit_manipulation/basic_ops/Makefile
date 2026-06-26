CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = bit_ops.out bit_ops_test.out

.PHONY: all clean test

all: $(TARGETS)

bit_ops.out: bit_ops.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

bit_ops_test.out: bit_ops_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: bit_ops_test.out
	./bit_ops_test.out

clean:
	rm -f $(TARGETS)
