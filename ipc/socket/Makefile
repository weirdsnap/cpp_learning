CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = socket_server socket_client socket_test

.PHONY: all clean test

all: $(TARGETS)

socket_server: server.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

socket_client: client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

socket_test: socket_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< -pthread

test: socket_test
	./socket_test

clean:
	rm -f $(TARGETS)
