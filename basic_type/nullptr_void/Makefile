CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = nullptr_void nullptr_void_test

.PHONY: all clean test

all: $(TARGETS)

nullptr_void: nullptr_void.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

nullptr_void_test: nullptr_void_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: nullptr_void_test
	./nullptr_void_test

clean:
	rm -f $(TARGETS)
