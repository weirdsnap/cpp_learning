CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = runtime_polymorphism.out runtime_polymorphism_test.out

.PHONY: all clean test

all: $(TARGETS)

runtime_polymorphism.out: runtime_polymorphism.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

runtime_polymorphism_test.out: runtime_polymorphism_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: runtime_polymorphism_test.out
	./runtime_polymorphism_test.out

clean:
	rm -f $(TARGETS)
