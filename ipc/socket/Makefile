CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest

TARGETS = socket_server.out socket_client.out socket_test.out

.PHONY: all clean test

all: $(TARGETS)

socket_server.out: server.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

socket_client.out: client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

socket_test.out: socket_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< -pthread

test: socket_test.out
	./socket_test.out

clean:
	rm -f $(TARGETS)
