CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = type_deduction type_deduction_test

.PHONY: all clean test

all: $(TARGETS)

type_deduction: type_deduction.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

type_deduction_test: type_deduction_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: type_deduction_test
	./type_deduction_test

clean:
	rm -f $(TARGETS)
