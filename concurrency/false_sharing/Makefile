CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: false_sharing.out false_sharing_test.out

false_sharing.out: false_sharing.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lpthread

false_sharing_test.out: false_sharing_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $< -lpthread

test: false_sharing_test.out
	./false_sharing_test.out

clean:
	rm -f false_sharing.out false_sharing_test.out
