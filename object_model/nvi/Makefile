CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = nvi nvi_test

.PHONY: all clean test

all: $(TARGETS)

nvi: nvi.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

nvi_test: nvi_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: nvi_test
	./nvi_test

clean:
	rm -f $(TARGETS)
