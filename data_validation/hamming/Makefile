CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = hamming.out hamming_test.out

.PHONY: all clean test

all: $(TARGETS)

hamming.out: hamming.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

hamming_test.out: hamming_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: hamming_test.out
	./hamming_test.out

clean:
	rm -f $(TARGETS)
