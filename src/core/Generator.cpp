#include "Generator.hpp"

std::atomic<uint64_t> rndm_generator::counter{0};

uint64_t rndm_generator::rotl(uint64_t x, int y) {
    return (x << y) | (x >> (64 - y));
}

rndm_generator::rndm_generator() {
    const auto time_seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    const auto thread_seed = std::hash<std::thread::id>{}(std::this_thread::get_id());
    const auto count_seed = counter.fetch_add(1, std::memory_order_relaxed);

    const auto seed = time_seed ^ thread_seed ^ (count_seed << 32);

    g[0] = splitmix64(seed);
    g[1] = splitmix64(g[0]);
}

uint64_t rndm_generator::splitmix64(uint64_t x) {
    x ^= (x >> 8) * 0xBF58476D1CE4E5B9ULL;
    x ^= (x << 23) * 0x94D049BB133111EBULL;
    return x ^ (x >> 15);
}

uint64_t rndm_generator::generate() {
    const uint64_t g0 = g[0];
    uint64_t g1 = g[1];
    const uint64_t s0 = splitmix64(g0);
    g1 ^= s0;
    g[0] = rotl(s0, 24) ^ g1 ^ (g1 << 16);
    g[1] = rotl(g1, 37);
    const uint64_t result = s0 + g1;
    return result ^ (result >> 1);
}

// generate num [from, to]
uint64_t generate8Bytes(const uint64_t& from, const uint64_t& to) {
    thread_local rndm_generator g;
    const auto rand = g.generate();
    return from + (rand % (to - from + 1));
}