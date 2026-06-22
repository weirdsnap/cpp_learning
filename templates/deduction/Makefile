CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = deduction deduction_test

.PHONY: all clean test

all: $(TARGETS)

deduction: deduction.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

deduction_test: deduction_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: deduction_test
	./deduction_test

clean:
	rm -f $(TARGETS)
