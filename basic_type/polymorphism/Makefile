CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = polymorphism polymorphism_test

.PHONY: all clean test

all: $(TARGETS)

polymorphism: polymorphism.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

polymorphism_test: polymorphism_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: polymorphism_test
	./polymorphism_test

clean:
	rm -f $(TARGETS)
