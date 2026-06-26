CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = exception_ub.out exception_ub_test.out

.PHONY: all clean test

all: $(TARGETS)

exception_ub.out: exception_ub.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

exception_ub_test.out: exception_ub_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: exception_ub_test.out
	./exception_ub_test.out

clean:
	rm -f $(TARGETS)
