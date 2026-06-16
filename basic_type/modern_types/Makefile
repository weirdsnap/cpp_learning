CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = modern_types modern_types_test

.PHONY: all clean test

all: $(TARGETS)

modern_types: modern_types.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

modern_types_test: modern_types_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: modern_types_test
	./modern_types_test

clean:
	rm -f $(TARGETS)
