CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = raii raii_test

.PHONY: all clean test

all: $(TARGETS)

raii: raii.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

raii_test: raii_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: raii_test
	./raii_test

clean:
	rm -f $(TARGETS)
