CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: bool_conversion bool_conversion_test

bool_conversion: bool_conversion.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

bool_conversion_test: bool_conversion_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: bool_conversion_test
	./bool_conversion_test

clean:
	rm -f bool_conversion bool_conversion_test
