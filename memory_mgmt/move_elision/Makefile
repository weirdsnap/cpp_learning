CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = move_elision move_elision_test

.PHONY: all clean test

all: $(TARGETS)

move_elision: move_elision.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

move_elision_test: move_elision_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: move_elision_test
	./move_elision_test

clean:
	rm -f $(TARGETS)
