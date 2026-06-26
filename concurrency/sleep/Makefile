CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: sleep.out sleep_test.out

sleep.out: sleep.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

sleep_test.out: sleep_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: sleep_test.out
	./sleep_test.out

clean:
	rm -f sleep.out sleep_test.out
