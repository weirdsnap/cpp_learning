CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -I ../../third_party/doctest

TARGETS = type_identity.out type_identity_test.out

.PHONY: all clean test

all: $(TARGETS)

type_identity.out: type_identity.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

type_identity_test.out: type_identity_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: type_identity_test.out
	./type_identity_test.out

clean:
	rm -f $(TARGETS)
