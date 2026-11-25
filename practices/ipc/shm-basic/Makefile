CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ../../../third_party/doctest
# -lrt 仅 Linux 需要（glibc 的 shm_open 在 librt）；macOS 在 libSystem 里
ifeq ($(shell uname),Linux)
LDFLAGS = -lrt -pthread
else
LDFLAGS = -pthread
endif

TARGETS = shm_basic_writer.out shm_basic_reader.out shm_basic_test.out

.PHONY: all clean test

all: $(TARGETS)

shm_basic_writer.out: writer.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_basic_reader.out: reader.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

shm_basic_test.out: shm_basic_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

test: shm_basic_test.out
	./shm_basic_test.out

clean:
	rm -f $(TARGETS)
