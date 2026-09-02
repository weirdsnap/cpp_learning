CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc422_test.out

.PHONY: all clean test

all: $(TARGETS)

lc422_test.out: lc422_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc422_test.out
	./lc422_test.out

clean:
	rm -f $(TARGETS)
