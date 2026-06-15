CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGETS = fundamental_types

.PHONY: all clean

all: $(TARGETS)

fundamental_types: fundamental_types.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
