CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../../third_party/doctest
# -lrt 仅 Linux 需要（glibc 的 shm_open 在 librt）；macOS 在 libSystem 里
ifeq ($(shell uname),Linux)
LDFLAGS = -lrt -pthread
else
LDFLAGS = -pthread
endif

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
