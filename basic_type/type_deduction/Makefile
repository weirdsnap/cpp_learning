CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = type_deduction.out type_deduction_test.out

.PHONY: all clean test

all: $(TARGETS)

type_deduction.out: type_deduction.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

type_deduction_test.out: type_deduction_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: type_deduction_test.out
	./type_deduction_test.out

clean:
	rm -f $(TARGETS)
