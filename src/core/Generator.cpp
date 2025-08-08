#include "Generator.hpp"

uint64_t generate8Bytes(const uint64_t& from, const uint64_t& to) {
    auto now = std::chrono::high_resolution_clock::now();
    uint64_t nanos = std::chrono::time_point_cast<std::chrono::nanoseconds>(now).time_since_epoch().count();

    nanos = (nanos >> 32) ^ (nanos & 0xFFFFFFFF);
    nanos = nanos * 0x45d9f3b;
    auto newRand = std::hash<std::uint64_t>{}(nanos);
    nanos = nanos ^ (nanos >> 16);

    return from + (nanos % (to - from + 1));
}