CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = minichat_server.out minichat_client.out minichat_test.out

.PHONY: all clean test

all: $(TARGETS)

minichat_server.out: server.cpp common.hpp
	$(CXX) $(CXXFLAGS) -o $@ server.cpp

minichat_client.out: client.cpp common.hpp
	$(CXX) $(CXXFLAGS) -o $@ client.cpp

minichat_test.out: minichat_test.cpp common.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -pthread

test: minichat_test.out
	./minichat_test.out

clean:
	rm -f $(TARGETS)
