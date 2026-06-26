CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = specialization.out specialization_test.out

.PHONY: all clean test

all: $(TARGETS)

specialization.out: specialization.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

specialization_test.out: specialization_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: specialization_test.out
	./specialization_test.out

clean:
	rm -f $(TARGETS)
