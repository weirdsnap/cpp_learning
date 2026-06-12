CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lrt

TARGETS = shm_basic_writer shm_basic_reader

.PHONY: all clean

all: $(TARGETS)

shm_basic_writer: writer.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_basic_reader: reader.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS)
