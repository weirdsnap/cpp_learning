CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: atomic_ref.out atomic_ref_test.out

atomic_ref.out: atomic_ref.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< -lpthread

atomic_ref_test.out: atomic_ref_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $< -lpthread

test: atomic_ref_test.out
	./atomic_ref_test.out

clean:
	rm -f atomic_ref.out atomic_ref_test.out
