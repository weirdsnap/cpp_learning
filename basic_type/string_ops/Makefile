CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = string_ops.out string_ops_test.out

.PHONY: all clean test

all: $(TARGETS)

string_ops.out: string_ops.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

string_ops_test.out: string_ops_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: string_ops_test.out
	./string_ops_test.out

clean:
	rm -f $(TARGETS)
