CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = crc32.out crc32_test.out

.PHONY: all clean test

all: $(TARGETS)

crc32.out: crc32.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

crc32_test.out: crc32_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: crc32_test.out
	./crc32_test.out

clean:
	rm -f $(TARGETS)
