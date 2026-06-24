CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest -pthread

TARGETS = basic_sync basic_sync_test

.PHONY: all clean test

all: $(TARGETS)

basic_sync: basic_sync.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

basic_sync_test: basic_sync_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: basic_sync_test
	./basic_sync_test

clean:
	rm -f $(TARGETS)
