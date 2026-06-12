CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lrt

TARGETS = unsign_int

.PHONY: all clean

all: $(TARGETS)

unsign_int: unsign_int.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS)
