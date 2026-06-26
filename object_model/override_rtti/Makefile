CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = override_rtti.out override_rtti_test.out

.PHONY: all clean test

all: $(TARGETS)

override_rtti.out: override_rtti.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

override_rtti_test.out: override_rtti_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: override_rtti_test.out
	./override_rtti_test.out

clean:
	rm -f $(TARGETS)
