CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: vptr_vbptr.out vptr_vbptr_test.out

vptr_vbptr.out: vptr_vbptr.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

vptr_vbptr_test.out: vptr_vbptr_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: vptr_vbptr_test.out
	./vptr_vbptr_test.out

clean:
	rm -f vptr_vbptr.out vptr_vbptr_test.out
