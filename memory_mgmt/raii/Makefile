CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = raii.out raii_test.out

.PHONY: all clean test

all: $(TARGETS)

raii.out: raii.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

raii_test.out: raii_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: raii_test.out
	./raii_test.out

clean:
	rm -f $(TARGETS)
