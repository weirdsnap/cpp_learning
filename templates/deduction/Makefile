CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = deduction.out deduction_test.out

.PHONY: all clean test

all: $(TARGETS)

deduction.out: deduction.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

deduction_test.out: deduction_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: deduction_test.out
	./deduction_test.out

clean:
	rm -f $(TARGETS)
