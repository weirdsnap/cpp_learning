CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = integer_promotion.out integer_promotion_test.out

.PHONY: all clean test

all: $(TARGETS)

integer_promotion.out: integer_promotion.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

integer_promotion_test.out: integer_promotion_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: integer_promotion_test.out
	./integer_promotion_test.out

clean:
	rm -f $(TARGETS)
