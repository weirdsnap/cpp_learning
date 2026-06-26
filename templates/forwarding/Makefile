CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = forwarding.out forwarding_test.out

.PHONY: all clean test

all: $(TARGETS)

forwarding.out: forwarding.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

forwarding_test.out: forwarding_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: forwarding_test.out
	./forwarding_test.out

clean:
	rm -f $(TARGETS)
