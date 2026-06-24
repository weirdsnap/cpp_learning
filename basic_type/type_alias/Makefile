CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = type_alias type_alias_test

.PHONY: all clean test

all: $(TARGETS)

type_alias: type_alias.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

type_alias_test: type_alias_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: type_alias_test
	./type_alias_test

clean:
	rm -f $(TARGETS)
