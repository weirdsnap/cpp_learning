CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = nvi.out nvi_test.out

.PHONY: all clean test

all: $(TARGETS)

nvi.out: nvi.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

nvi_test.out: nvi_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: nvi_test.out
	./nvi_test.out

clean:
	rm -f $(TARGETS)
