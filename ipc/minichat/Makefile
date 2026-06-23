CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = minichat_server minichat_client minichat_test

.PHONY: all clean test

all: $(TARGETS)

minichat_server: server.cpp common.hpp
	$(CXX) $(CXXFLAGS) -o $@ server.cpp

minichat_client: client.cpp common.hpp
	$(CXX) $(CXXFLAGS) -o $@ client.cpp

minichat_test: minichat_test.cpp common.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -pthread

test: minichat_test
	./minichat_test

clean:
	rm -f $(TARGETS)
