CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = fundamental_types fundamental_types_test

.PHONY: all clean test

all: $(TARGETS)

fundamental_types: fundamental_types.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

fundamental_types_test: fundamental_types_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: fundamental_types_test
	./fundamental_types_test

clean:
	rm -f $(TARGETS)
