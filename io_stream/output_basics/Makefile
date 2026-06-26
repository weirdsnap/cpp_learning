CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: output_basics.out output_basics_test.out

output_basics.out: output_basics.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

output_basics_test.out: output_basics_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: output_basics_test.out
	./output_basics_test.out

clean:
	rm -f output_basics.out output_basics_test.out
