CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = ownership ownership_test

.PHONY: all clean test

all: $(TARGETS)

ownership: ownership.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ownership_test: ownership_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: ownership_test
	./ownership_test

clean:
	rm -f $(TARGETS)
