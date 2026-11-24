CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = moved_from.out moved_from_test.out

.PHONY: all clean test

all: $(TARGETS)

moved_from.out: moved_from.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

moved_from_test.out: moved_from_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: moved_from_test.out
	./moved_from_test.out

clean:
	rm -f $(TARGETS)
