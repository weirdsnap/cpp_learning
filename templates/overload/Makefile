CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = overload overload_test

.PHONY: all clean test

all: $(TARGETS)

overload: overload.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

overload_test: overload_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: overload_test
	./overload_test

clean:
	rm -f $(TARGETS)
