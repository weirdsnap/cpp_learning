CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = variadic variadic_test

.PHONY: all clean test

all: $(TARGETS)

variadic: variadic.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

variadic_test: variadic_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: variadic_test
	./variadic_test

clean:
	rm -f $(TARGETS)
