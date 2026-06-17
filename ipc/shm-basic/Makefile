CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../third_party/doctest
LDFLAGS = -lrt -pthread

TARGETS = shm_basic_writer shm_basic_reader shm_basic_test

.PHONY: all clean test

all: $(TARGETS)

shm_basic_writer: writer.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_basic_reader: reader.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_basic_test: shm_basic_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

test: shm_basic_test
	./shm_basic_test

clean:
	rm -f $(TARGETS)
