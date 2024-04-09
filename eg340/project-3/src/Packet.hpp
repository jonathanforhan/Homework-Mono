#pragma once

#include <bitset>

/// @brief 10-bit packet
struct Packet : public std::bitset<10> {
    static constexpr std::size_t CHKBIT = 8; ///< checksum bit index
    static constexpr std::size_t ACKBIT = 9; ///< ack bit index

    explicit Packet(auto c)
        : std::bitset<10>(c) {}

    /// @brief set checksum bit using parent set() method
    /// @param b boolean
    void set_chkbit(bool b = true) { this->set(CHKBIT, b); }

    /// @brief set ack bit using parent set() method
    /// @param b boolean
    void set_ackbit(bool b = true) { this->set(ACKBIT, b); }

    /// @brief QOL improvement
    /// @return first 8 bits as char
    char to_char() const { return static_cast<char>(this->to_ulong()); }

    /// @brief calculates checksum from first 8 bits
    /// @param pkt packet to calc
    /// @return checksum bit state (0|1)
    static bool checksum(const Packet& pkt) noexcept {
        uint8_t byte = static_cast<uint8_t>(pkt.to_ulong());
        byte ^= byte >> 4;
        byte ^= byte >> 2;
        byte ^= byte >> 1;
        return byte;
    }
};
