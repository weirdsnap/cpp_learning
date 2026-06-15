CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGETS = modern_types

.PHONY: all clean

all: $(TARGETS)

modern_types: modern_types.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
