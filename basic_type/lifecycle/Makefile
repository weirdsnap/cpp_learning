CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-reorder -Wno-return-local-addr -I ../../third_party/doctest

TARGETS = lifecycle lifecycle_test

.PHONY: all clean test

all: $(TARGETS)

lifecycle: lifecycle.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

lifecycle_test: lifecycle_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: lifecycle_test
	./lifecycle_test

clean:
	rm -f $(TARGETS)
