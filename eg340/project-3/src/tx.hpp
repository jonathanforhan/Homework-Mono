#pragma once

#include <cstdio>
#include <filesystem>
#include "util.hpp"

#define CACHE_SIZE 1024 * 32

class Transmitter {
public:
    explicit Transmitter(const std::filesystem::path& file_path);

    ~Transmitter();

    void read_packet() noexcept;

    constexpr const Packet& transmit() const noexcept { return m_packet_cache[m_current_packet]; }

    void receive(const Packet& packet) noexcept {
        m_transmitting = !packet[ACK_BIT];
    }

    constexpr bool transmitting() const noexcept { return m_transmitting; }

private:
    FILE* m_file;
    Packet m_packet_cache[CACHE_SIZE];
    size_t m_cache_in_use = 0;
    size_t m_current_packet = 0;
    bool m_transmitting;
};
