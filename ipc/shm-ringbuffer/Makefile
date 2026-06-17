CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include -I ../../third_party/doctest
LDFLAGS = -lrt -pthread

TARGETS = shm_ringbuffer_writer shm_ringbuffer_reader shm_ringbuffer_test

.PHONY: all clean test

all: $(TARGETS)

shm_ringbuffer_writer: writer.cpp include/shm_data.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_ringbuffer_reader: reader.cpp include/shm_data.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_ringbuffer_test: shm_ringbuffer_test.cpp include/shm_data.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

test: shm_ringbuffer_test
	./shm_ringbuffer_test

clean:
	rm -f $(TARGETS)
