#pragma once

/// @file Channel.hpp
/// @brief Channel that bridges transmitter and receiver and applies noise

#include "Packet.hpp"
#include "Receiver.hpp"
#include "Transmitter.hpp"

/// @brief performance tracker
struct PerfLog {
    uint64_t noise = 0;           /// % noise
    uint64_t transmissions = 0;   /// total transmissions
    uint64_t retransmissions = 0; /// total retramissions
    uint64_t good_true = 0;       /// good signal accepted
    uint64_t good_false = 0;      /// good signal rejected
    uint64_t bad_true = 0;        /// bad signal accepted
    uint64_t bad_false = 0;       /// bad signal rejected
    uint64_t first_tries = 0;     /// good and accepted first try
    uint64_t catches = 0;         /// good and accepted after a/many retranmission(s)
    uint64_t tx_errors = 0;       /// tx ack bit errors
    uint64_t rx_errors = 0;       /// rx ack bit errors
};

/// @brief Channel that applies noise between the communication of tx and rx
/// Channel also records data of if transmissions are successful
class Channel {
public:
    explicit Channel(uint32_t n)
        : _n(n) {}

    /// @brief initate communication over channel
    /// @param tx transmitter with valid file
    /// @param rx receiver with valid file
    void communicate(Transmitter& tx, Receiver& rx);

    /// @brief reset all perf tracker data
    void reset_tracker() { tracker = {}; }

public:
    /// @brief track for channel performance data
    /// recorded on Channel::communicate method
    PerfLog tracker;

private:
    Packet scramble(Packet pkt, uint32_t n);

private:
    uint32_t _n;
};
