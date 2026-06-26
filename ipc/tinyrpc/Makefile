CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = tinyrpc_server.out tinyrpc_client.out tinyrpc_test.out

.PHONY: all clean test

all: $(TARGETS)

tinyrpc_server.out: server.cpp rpc_protocol.hpp
	$(CXX) $(CXXFLAGS) -o $@ server.cpp

tinyrpc_client.out: client.cpp rpc_protocol.hpp
	$(CXX) $(CXXFLAGS) -o $@ client.cpp

tinyrpc_test.out: tinyrpc_test.cpp rpc_protocol.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -pthread

test: tinyrpc_test.out
	./tinyrpc_test.out

clean:
	rm -f $(TARGETS)
