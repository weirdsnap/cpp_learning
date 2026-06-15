CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGETS = type_deduction

.PHONY: all clean

all: $(TARGETS)

type_deduction: type_deduction.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
