CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: vptr_vbptr vptr_vbptr_test

vptr_vbptr: vptr_vbptr.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

vptr_vbptr_test: vptr_vbptr_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: vptr_vbptr_test
	./vptr_vbptr_test

clean:
	rm -f vptr_vbptr vptr_vbptr_test
