CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: sleep sleep_test

sleep: sleep.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

sleep_test: sleep_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: sleep_test
	./sleep_test

clean:
	rm -f sleep sleep_test
