CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc487_test.out

.PHONY: all clean test

all: $(TARGETS)

lc487_test.out: lc487_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc487_test.out
	./lc487_test.out

clean:
	rm -f $(TARGETS)
