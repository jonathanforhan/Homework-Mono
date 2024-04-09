#pragma once

/// @file Transmitter.hpp
/// @brief Transmitter object

#include <filesystem>
#include <fstream>
#include "Packet.hpp"

/// @brief transmits file over a channel
class Transmitter {
public:
    /// @brief transmitter that transmits bytes from stored file
    /// @param file_path path to text file to transmit
    explicit Transmitter(const std::filesystem::path& file_path)
        : _fin(file_path) {}

    /// @brief transmit next char in stored file
    /// @return packet with proper checksum and ack bit set to zero, no noise appied yet
    Packet transmit() {
        char c;
        _fin.get(c);

        Packet pkt(c);
        pkt.set_chkbit(Packet::checksum(pkt));

        return pkt;
    }

    /// @brief describes state of transmission, EOT is end of transmission
    /// @return state of transmission
    constexpr bool eot() const { return !_fin; }

private:
    std::ifstream _fin;
};
