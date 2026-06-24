CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = string_ops string_ops_test

.PHONY: all clean test

all: $(TARGETS)

string_ops: string_ops.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

string_ops_test: string_ops_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: string_ops_test
	./string_ops_test

clean:
	rm -f $(TARGETS)
