CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: bool_conversion.out bool_conversion_test.out

bool_conversion.out: bool_conversion.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

bool_conversion_test.out: bool_conversion_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: bool_conversion_test.out
	./bool_conversion_test.out

clean:
	rm -f bool_conversion.out bool_conversion_test.out
