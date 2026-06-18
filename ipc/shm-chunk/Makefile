CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest
LDFLAGS = -lrt -pthread

TARGETS = shm_chunk_productor shm_chunk_consumer shm_chunk_test

.PHONY: all clean test

all: $(TARGETS)

shm_chunk_productor: shm_chunk_productor.cpp shm_chunk.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_chunk_consumer: shm_chunk_consumer.cpp shm_chunk.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_chunk_test: shm_chunk_test.cpp shm_chunk.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

test: shm_chunk_test
	./shm_chunk_test

clean:
	rm -f $(TARGETS)
