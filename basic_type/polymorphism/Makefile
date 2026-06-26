CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = polymorphism.out polymorphism_test.out

.PHONY: all clean test

all: $(TARGETS)

polymorphism.out: polymorphism.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

polymorphism_test.out: polymorphism_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: polymorphism_test.out
	./polymorphism_test.out

clean:
	rm -f $(TARGETS)
