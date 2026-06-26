CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest
LDFLAGS = -lrt -pthread

TARGETS = shm_chunk_productor.out shm_chunk_consumer.out shm_chunk_bench.out shm_chunk_test.out

.PHONY: all clean test bench

all: $(TARGETS)

shm_chunk_productor.out: shm_chunk_productor.cpp shm_chunk.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_chunk_consumer.out: shm_chunk_consumer.cpp shm_chunk.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_chunk_bench.out: shm_chunk_bench.cpp shm_chunk.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_chunk_test.out: shm_chunk_test.cpp shm_chunk.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

test: shm_chunk_test.out
	./shm_chunk_test.out

bench: shm_chunk_bench.out
	./shm_chunk_bench.out

clean:
	rm -f $(TARGETS)
