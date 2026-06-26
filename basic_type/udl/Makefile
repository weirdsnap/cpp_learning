CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: udl.out udl_test.out

udl.out: udl.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

udl_test.out: udl_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: udl_test.out
	./udl_test.out

clean:
	rm -f udl.out udl_test.out
