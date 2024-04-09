#pragma once

/// @file Channel.hpp
/// @brief Channel that bridges transmitter and receiver and applies noise

#include "Packet.hpp"
#include "Receiver.hpp"
#include "Transmitter.hpp"

/// @brief performance tracker
struct PerfLog {
    uint32_t noise = 0;           /// % noise
    uint32_t transmissions = 0;   /// total transmissions
    uint32_t retransmissions = 0; /// total retramissions
    uint32_t good_true = 0;       /// good signal accepted
    uint32_t good_false = 0;      /// good signal rejected
    uint32_t bad_true = 0;        /// bad signal accepted
    uint32_t bad_false = 0;       /// bad signal rejected
    uint32_t first_tries = 0;     /// good and accepted first try
    uint32_t catches = 0;         /// good and accepted after a/many retranmission(s)
    uint32_t tx_errors = 0;       /// tx ack bit errors
    uint32_t rx_errors = 0;       /// rx ack bit errors
};

/// @brief Channel that applies noise between the communication of tx and rx
/// Channel also records data of if transmissions are successful
class Channel {
public:
    /// @brief Channel object that facilitates communication between tx and rx
    /// @param n % noise
    explicit Channel(uint32_t n)
        : _n(n) {}

    /// @brief initate communication over channel
    /// @param tx transmitter with valid file
    /// @param rx receiver with valid file
    void communicate(Transmitter& tx, Receiver& rx);

public:
    /// @brief track for channel performance data
    /// recorded on Channel::communicate method
    PerfLog tracker;

private:
    Packet scramble(Packet pkt);

private:
    uint32_t _n;
};
