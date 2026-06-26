CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = parity_xor.out parity_xor_test.out

.PHONY: all clean test

all: $(TARGETS)

parity_xor.out: parity_xor.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

parity_xor_test.out: parity_xor_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: parity_xor_test.out
	./parity_xor_test.out

clean:
	rm -f $(TARGETS)
