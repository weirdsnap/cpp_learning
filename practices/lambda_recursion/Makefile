CXX = g++
CXXFLAGS := -std=c++23 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lambda_recursion_test.out

.PHONY: all clean test

all: $(TARGETS)

lambda_recursion_test.out: lambda_recursion_test.cpp lambda_recursion.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lambda_recursion_test.out
	./lambda_recursion_test.out

clean:
	rm -f $(TARGETS)
