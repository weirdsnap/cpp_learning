CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = hamming hamming_test

.PHONY: all clean test

all: $(TARGETS)

hamming: hamming.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

hamming_test: hamming_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: hamming_test
	./hamming_test

clean:
	rm -f $(TARGETS)
