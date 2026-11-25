CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = type_erasure.out type_erasure_test.out

.PHONY: all clean test

all: $(TARGETS)

type_erasure.out: type_erasure.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

type_erasure_test.out: type_erasure_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: type_erasure_test.out
	./type_erasure_test.out

clean:
	rm -f $(TARGETS)
