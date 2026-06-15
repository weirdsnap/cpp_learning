CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGETS = function_lambda

.PHONY: all clean

all: $(TARGETS)

function_lambda: function_lambda.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
