CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = variadic.out variadic_test.out

.PHONY: all clean test

all: $(TARGETS)

variadic.out: variadic.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

variadic_test.out: variadic_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: variadic_test.out
	./variadic_test.out

clean:
	rm -f $(TARGETS)
