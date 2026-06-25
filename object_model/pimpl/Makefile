CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: pimpl_demo pimpl_test

pimpl_demo: pimpl_demo.cpp widget.cpp widget.h
	$(CXX) $(CXXFLAGS) -o $@ pimpl_demo.cpp widget.cpp

pimpl_test: pimpl_test.cpp widget.cpp widget.h
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ pimpl_test.cpp widget.cpp

test: pimpl_test
	./pimpl_test

clean:
	rm -f pimpl_demo pimpl_test
