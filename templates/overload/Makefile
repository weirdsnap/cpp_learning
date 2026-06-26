CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = overload.out overload_test.out

.PHONY: all clean test

all: $(TARGETS)

overload.out: overload.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

overload_test.out: overload_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: overload_test.out
	./overload_test.out

clean:
	rm -f $(TARGETS)
