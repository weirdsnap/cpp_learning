CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-reorder -I ../../third_party/doctest

TARGETS = initialization.out initialization_test.out

.PHONY: all clean test

all: $(TARGETS)

initialization.out: initialization.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

initialization_test.out: initialization_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: initialization_test.out
	./initialization_test.out

clean:
	rm -f $(TARGETS)
