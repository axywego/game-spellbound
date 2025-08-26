#pragma once 

#include <atomic>
#include <chrono>
#include <thread>

// mine remake Xoroshiro128+ algorithm
class rndm_generator {
    uint64_t g[2]{};
    static std::atomic<uint64_t> counter;

    static uint64_t rotl(uint64_t x, int y);
public:
    rndm_generator();

    static uint64_t splitmix64(uint64_t x);

    uint64_t generate();
};

uint64_t generate8Bytes(const uint64_t& from, const uint64_t& to);