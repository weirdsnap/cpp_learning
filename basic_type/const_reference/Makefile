CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-dangling-reference -I ../../third_party/doctest

TARGETS = const_reference.out const_reference_test.out

.PHONY: all clean test

all: $(TARGETS)

const_reference.out: const_reference.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

const_reference_test.out: const_reference_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: const_reference_test.out
	./const_reference_test.out

clean:
	rm -f $(TARGETS)
