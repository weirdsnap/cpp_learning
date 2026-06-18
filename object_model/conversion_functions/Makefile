CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = conversion_functions conversion_functions_test

.PHONY: all clean test

all: $(TARGETS)

conversion_functions: conversion_functions.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

conversion_functions_test: conversion_functions_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: conversion_functions_test
	./conversion_functions_test

clean:
	rm -f $(TARGETS)
