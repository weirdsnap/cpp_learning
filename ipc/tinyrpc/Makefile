CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = tinyrpc_server tinyrpc_client tinyrpc_test

.PHONY: all clean test

all: $(TARGETS)

tinyrpc_server: server.cpp rpc_protocol.hpp
	$(CXX) $(CXXFLAGS) -o $@ server.cpp

tinyrpc_client: client.cpp rpc_protocol.hpp
	$(CXX) $(CXXFLAGS) -o $@ client.cpp

tinyrpc_test: tinyrpc_test.cpp rpc_protocol.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< -pthread

test: tinyrpc_test
	./tinyrpc_test

clean:
	rm -f $(TARGETS)
