CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = override_rtti override_rtti_test

.PHONY: all clean test

all: $(TARGETS)

override_rtti: override_rtti.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

override_rtti_test: override_rtti_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: override_rtti_test
	./override_rtti_test

clean:
	rm -f $(TARGETS)
