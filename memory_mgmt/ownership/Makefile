CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = ownership.out ownership_test.out

.PHONY: all clean test

all: $(TARGETS)

ownership.out: ownership.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ownership_test.out: ownership_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: ownership_test.out
	./ownership_test.out

clean:
	rm -f $(TARGETS)
