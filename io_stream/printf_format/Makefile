CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra
DOCTEST  := -I ../../third_party/doctest

.PHONY: all test clean

all: printf_format.out printf_format_test.out

printf_format.out: printf_format.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

printf_format_test.out: printf_format_test.cpp
	$(CXX) $(CXXFLAGS) $(DOCTEST) -o $@ $<

test: printf_format_test.out
	./printf_format_test.out

clean:
	rm -f printf_format.out printf_format_test.out
