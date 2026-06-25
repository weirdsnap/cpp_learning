CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: udl udl_test

udl: udl.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

udl_test: udl_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: udl_test
	./udl_test

clean:
	rm -f udl udl_test
