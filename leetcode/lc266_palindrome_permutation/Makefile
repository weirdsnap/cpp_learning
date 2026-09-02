CXX = g++
CXXFLAGS := -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = lc266_test.out

.PHONY: all clean test

all: $(TARGETS)

lc266_test.out: lc266_test.cpp solution.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lc266_test.out
	./lc266_test.out

clean:
	rm -f $(TARGETS)
