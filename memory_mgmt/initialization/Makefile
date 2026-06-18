CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-reorder -I ../../third_party/doctest

TARGETS = initialization initialization_test

.PHONY: all clean test

all: $(TARGETS)

initialization: initialization.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

initialization_test: initialization_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: initialization_test
	./initialization_test

clean:
	rm -f $(TARGETS)
