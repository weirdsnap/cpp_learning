CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-reorder -Wno-return-local-addr

TARGETS = lifecycle

.PHONY: all clean

all: $(TARGETS)

lifecycle: lifecycle.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
