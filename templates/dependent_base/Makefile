CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = dependent_base.out dependent_base_test.out

.PHONY: all clean test

all: $(TARGETS)

dependent_base.out: dependent_base.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

dependent_base_test.out: dependent_base_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: dependent_base_test.out
	./dependent_base_test.out

clean:
	rm -f $(TARGETS)
