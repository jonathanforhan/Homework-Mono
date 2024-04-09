#pragma once

/// @file Receiver.hpp
/// @brief Receiver object

#include <filesystem>
#include <fstream>
#include <optional>
#include "Packet.hpp"

/// @brief receives packets from transmitter over channel
/// receiver will write to an output file
class Receiver {
public:
    /// @brief creates receiver and opens output file
    /// @param file_name desired name out output file
    /// @param write should write to file?
    Receiver(const std::filesystem::path& file_name, bool write = false)
        : _write(write) {
        if (_write)
            _fout = {file_name};
    }

    /// @brief receive a packet from the channel
    /// @param pkt
    /// @return returns a packet with ack=1 indicating retransmit request OR nullopt, an implicit handshake
    std::optional<Packet> receive(Packet pkt) {
        if (pkt[Packet::CHKBIT] != Packet::checksum(pkt)) {
            pkt.set_ackbit();
            return pkt;
        }

        if (_write)
            _fout << pkt.to_char();

        return std::nullopt;
    }

private:
    std::ofstream _fout;
    bool _write;
};
