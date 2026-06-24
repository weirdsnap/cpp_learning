CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = if_constexpr if_constexpr_test

.PHONY: all clean test

all: $(TARGETS)

if_constexpr: if_constexpr.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

if_constexpr_test: if_constexpr_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: if_constexpr_test
	./if_constexpr_test

clean:
	rm -f $(TARGETS)
