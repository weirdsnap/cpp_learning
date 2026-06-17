#pragma once
#ifndef SHM_DATA_H
#define SHM_DATA_H

#include <atomic>
#include <cstddef>

constexpr const char* NAME = "/my_shm_ringbuffer";
constexpr int MSG_COUNT = 100;

struct Message {
    int id;
    char name[32];
    double value;
};

struct Data {
    std::atomic<int> write_idx{0};
    std::atomic<int> read_idx{0};
    Message msgs[MSG_COUNT];
};

constexpr size_t SHM_SIZE = sizeof(Data);

#endif
