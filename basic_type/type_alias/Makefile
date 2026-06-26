CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = type_alias.out type_alias_test.out

.PHONY: all clean test

all: $(TARGETS)

type_alias.out: type_alias.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

type_alias_test.out: type_alias_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: type_alias_test.out
	./type_alias_test.out

clean:
	rm -f $(TARGETS)
