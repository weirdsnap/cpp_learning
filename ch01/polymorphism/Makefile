CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGETS = polymorphism

.PHONY: all clean

all: $(TARGETS)

polymorphism: polymorphism.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
