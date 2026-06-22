CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = parity_xor parity_xor_test

.PHONY: all clean test

all: $(TARGETS)

parity_xor: parity_xor.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

parity_xor_test: parity_xor_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: parity_xor_test
	./parity_xor_test

clean:
	rm -f $(TARGETS)
