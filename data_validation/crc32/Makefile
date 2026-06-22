CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = crc32 crc32_test

.PHONY: all clean test

all: $(TARGETS)

crc32: crc32.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

crc32_test: crc32_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: crc32_test
	./crc32_test

clean:
	rm -f $(TARGETS)
