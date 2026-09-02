CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc252_test.out

.PHONY: all clean test

all: $(TARGETS)

lc252_test.out: lc252_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc252_test.out
	./lc252_test.out

clean:
	rm -f $(TARGETS)
