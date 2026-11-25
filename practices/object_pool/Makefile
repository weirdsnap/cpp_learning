CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I ../../third_party/doctest

TARGETS = object_pool_test.out bench.out

.PHONY: all clean test bench

all: $(TARGETS)

object_pool_test.out: object_pool_test.cpp object_pool.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

bench.out: bench.cpp object_pool.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: object_pool_test.out
	./object_pool_test.out

bench: bench.out
	./bench.out

clean:
	rm -f $(TARGETS)
