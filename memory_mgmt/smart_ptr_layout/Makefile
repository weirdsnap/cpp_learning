CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = smart_ptr_layout.out smart_ptr_layout_test.out

.PHONY: all clean test

all: $(TARGETS)

smart_ptr_layout.out: smart_ptr_layout.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

smart_ptr_layout_test.out: smart_ptr_layout_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: smart_ptr_layout_test.out
	./smart_ptr_layout_test.out

clean:
	rm -f $(TARGETS)
