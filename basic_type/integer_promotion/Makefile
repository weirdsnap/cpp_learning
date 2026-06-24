CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = integer_promotion integer_promotion_test

.PHONY: all clean test

all: $(TARGETS)

integer_promotion: integer_promotion.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

integer_promotion_test: integer_promotion_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: integer_promotion_test
	./integer_promotion_test

clean:
	rm -f $(TARGETS)
