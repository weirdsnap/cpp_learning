CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = specialization specialization_test

.PHONY: all clean test

all: $(TARGETS)

specialization: specialization.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

specialization_test: specialization_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: specialization_test
	./specialization_test

clean:
	rm -f $(TARGETS)
