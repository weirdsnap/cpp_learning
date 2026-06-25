CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: output_basics output_basics_test

output_basics: output_basics.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

output_basics_test: output_basics_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: output_basics_test
	./output_basics_test

clean:
	rm -f output_basics output_basics_test
