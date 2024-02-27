#pragma once

#include <bitset>
#include <cstdint>
#include <ctime>

#define ACK_BIT 8
#define CHECKSUM_BIT 9

using Packet = std::bitset<10>;

// https://www.reedbeta.com/blog/hash-functions-for-gpu-rendering/
static inline uint32_t pcg_hash() noexcept {
    static uint32_t s_seed = std::time(nullptr);
    uint32_t state = s_seed * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    s_seed = (word >> 22u) ^ word;
    return s_seed;
}

constexpr bool checksum(uint8_t byte) noexcept {
    byte ^= byte >> 4;
    byte ^= byte >> 2;
    byte ^= byte >> 1;
    return byte & 1;
}

constexpr inline void scramble(Packet& packet, uint32_t odds, uint32_t random_number) noexcept {
    if (random_number < odds) {
        packet[random_number % 10].flip();
    }
}
