CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = name_hiding name_hiding_test

.PHONY: all clean test

all: $(TARGETS)

name_hiding: name_hiding.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

name_hiding_test: name_hiding_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: name_hiding_test
	./name_hiding_test

clean:
	rm -f $(TARGETS)
