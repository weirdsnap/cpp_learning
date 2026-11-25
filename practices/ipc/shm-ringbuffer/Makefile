CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include -I ../../../third_party/doctest
# -lrt 仅 Linux 需要（glibc 的 shm_open 在 librt）；macOS 在 libSystem 里
ifeq ($(shell uname),Linux)
LDFLAGS = -lrt -pthread
else
LDFLAGS = -pthread
endif

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
