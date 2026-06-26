CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-reorder -Wno-return-local-addr -I ../../third_party/doctest

TARGETS = lifecycle.out lifecycle_test.out

.PHONY: all clean test

all: $(TARGETS)

lifecycle.out: lifecycle.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

lifecycle_test.out: lifecycle_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lifecycle_test.out
	./lifecycle_test.out

clean:
	rm -f $(TARGETS)
