CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = virtual_dispatch virtual_dispatch_test

.PHONY: all clean test

all: $(TARGETS)

virtual_dispatch: virtual_dispatch.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

virtual_dispatch_test: virtual_dispatch_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: virtual_dispatch_test
	./virtual_dispatch_test

clean:
	rm -f $(TARGETS)
