CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = virtual_dispatch.out virtual_dispatch_test.out

.PHONY: all clean test

all: $(TARGETS)

virtual_dispatch.out: virtual_dispatch.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

virtual_dispatch_test.out: virtual_dispatch_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: virtual_dispatch_test.out
	./virtual_dispatch_test.out

clean:
	rm -f $(TARGETS)
