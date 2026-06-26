CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = move_elision.out move_elision_test.out

.PHONY: all clean test

all: $(TARGETS)

move_elision.out: move_elision.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

move_elision_test.out: move_elision_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: move_elision_test.out
	./move_elision_test.out

clean:
	rm -f $(TARGETS)
