CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc877_test.out

.PHONY: all clean test

all: $(TARGETS)

lc877_test.out: lc877_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc877_test.out
	./lc877_test.out

clean:
	rm -f $(TARGETS)
