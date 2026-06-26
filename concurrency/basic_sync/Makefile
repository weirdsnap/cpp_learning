CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest -pthread

TARGETS = basic_sync.out basic_sync_test.out

.PHONY: all clean test

all: $(TARGETS)

basic_sync.out: basic_sync.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

basic_sync_test.out: basic_sync_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: basic_sync_test.out
	./basic_sync_test.out

clean:
	rm -f $(TARGETS)
