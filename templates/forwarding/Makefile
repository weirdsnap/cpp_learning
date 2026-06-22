CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = forwarding forwarding_test

.PHONY: all clean test

all: $(TARGETS)

forwarding: forwarding.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

forwarding_test: forwarding_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: forwarding_test
	./forwarding_test

clean:
	rm -f $(TARGETS)
