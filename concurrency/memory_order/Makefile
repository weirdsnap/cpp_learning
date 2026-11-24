CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: memory_order.out memory_order_test.out

memory_order.out: memory_order.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lpthread

memory_order_test.out: memory_order_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $< -lpthread

test: memory_order_test.out
	./memory_order_test.out

clean:
	rm -f memory_order.out memory_order_test.out
