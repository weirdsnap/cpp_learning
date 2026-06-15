CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-dangling-reference

TARGETS = const_reference

.PHONY: all clean

all: $(TARGETS)

const_reference: const_reference.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
