CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = exception_ub exception_ub_test

.PHONY: all clean test

all: $(TARGETS)

exception_ub: exception_ub.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

exception_ub_test: exception_ub_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: exception_ub_test
	./exception_ub_test

clean:
	rm -f $(TARGETS)
