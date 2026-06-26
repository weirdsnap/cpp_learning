CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = virtual_inheritance.out virtual_inheritance_test.out

.PHONY: all clean test

all: $(TARGETS)

virtual_inheritance.out: virtual_inheritance.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

virtual_inheritance_test.out: virtual_inheritance_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: virtual_inheritance_test.out
	./virtual_inheritance_test.out

clean:
	rm -f $(TARGETS)
