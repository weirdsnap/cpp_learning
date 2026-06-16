CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGETS = nullptr_void

.PHONY: all clean

all: $(TARGETS)

nullptr_void: nullptr_void.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
