CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = smart_pointers smart_pointers_test

.PHONY: all clean test

all: $(TARGETS)

smart_pointers: smart_pointers.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

smart_pointers_test: smart_pointers_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: smart_pointers_test
	./smart_pointers_test

clean:
	rm -f $(TARGETS)
