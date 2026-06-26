CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include -I ../../third_party/doctest
LDFLAGS = -lrt -pthread

TARGETS = shm_ringbuffer_writer.out shm_ringbuffer_reader.out shm_ringbuffer_test.out

.PHONY: all clean test

all: $(TARGETS)

shm_ringbuffer_writer.out: writer.cpp include/shm_data.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_ringbuffer_reader.out: reader.cpp include/shm_data.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_ringbuffer_test.out: shm_ringbuffer_test.cpp include/shm_data.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

test: shm_ringbuffer_test.out
	./shm_ringbuffer_test.out

clean:
	rm -f $(TARGETS)
