CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = bit_ops bit_ops_test

.PHONY: all clean test

all: $(TARGETS)

bit_ops: bit_ops.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

bit_ops_test: bit_ops_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: bit_ops_test
	./bit_ops_test

clean:
	rm -f $(TARGETS)
