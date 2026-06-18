CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = virtual_inheritance virtual_inheritance_test

.PHONY: all clean test

all: $(TARGETS)

virtual_inheritance: virtual_inheritance.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

virtual_inheritance_test: virtual_inheritance_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: virtual_inheritance_test
	./virtual_inheritance_test

clean:
	rm -f $(TARGETS)
