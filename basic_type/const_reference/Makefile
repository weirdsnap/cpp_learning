CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-dangling-reference -I ../../third_party/doctest

TARGETS = const_reference const_reference_test

.PHONY: all clean test

all: $(TARGETS)

const_reference: const_reference.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

const_reference_test: const_reference_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: const_reference_test
	./const_reference_test

clean:
	rm -f $(TARGETS)
