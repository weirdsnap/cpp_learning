CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include
LDFLAGS = -lrt -pthread

TARGETS = shm_ringbuffer_writer shm_ringbuffer_reader

.PHONY: all clean

all: $(TARGETS)

shm_ringbuffer_writer: writer.cpp include/shm_data.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_ringbuffer_reader: reader.cpp include/shm_data.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS)
