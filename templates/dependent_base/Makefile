CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = dependent_base dependent_base_test

.PHONY: all clean test

all: $(TARGETS)

dependent_base: dependent_base.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

dependent_base_test: dependent_base_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: dependent_base_test
	./dependent_base_test

clean:
	rm -f $(TARGETS)
