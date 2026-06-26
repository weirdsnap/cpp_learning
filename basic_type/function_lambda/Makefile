CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = function_lambda.out function_lambda_test.out

.PHONY: all clean test

all: $(TARGETS)

function_lambda.out: function_lambda.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

function_lambda_test.out: function_lambda_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: function_lambda_test.out
	./function_lambda_test.out

clean:
	rm -f $(TARGETS)
