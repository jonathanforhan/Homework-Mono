#include "Channel.hpp"

#include <random>

static thread_local std::random_device rd;
static thread_local std::mt19937 mt(rd());
static thread_local std::uniform_int_distribution<uint32_t> dist(0, 100);

void Channel::communicate(Transmitter& tx, Receiver& rx) {
    tracker.noise = _n;

    for (;;) {
        Packet pkt = tx.transmit();
        if (tx.eot())
            break;

        uint32_t transmissions = 0;
        bool good, accepted;

        do {
            Packet noisy_pkt = scramble(pkt, _n);
            std::optional<Packet> res = rx.receive(noisy_pkt);

            // record tracker data
            good = pkt.to_char() == noisy_pkt.to_char();
            accepted = res == std::nullopt;

            // good/bad true/false
            tracker.good_true += good & accepted;
            tracker.good_false += good & (!accepted);
            tracker.bad_true += (!good) & accepted;
            tracker.bad_false += (!good) & (!accepted);

            // tx rx ack errors
            tracker.tx_errors += noisy_pkt[Packet::ACKBIT];
            if (!accepted) {
                Packet noisy_res = scramble(*res, _n);
                tracker.rx_errors += !noisy_res[Packet::ACKBIT];
            }

            transmissions++;
        } while (!accepted);

        tracker.first_tries += good & (transmissions == 1);
        tracker.catches += good & (transmissions != 1);

        tracker.transmissions += transmissions;
        tracker.retransmissions += transmissions - 1;
    }
}

Packet Channel::scramble(Packet pkt, uint32_t n) {
    // apply noise to packet
    for (uint32_t i = 0; i < pkt.size(); i++) {
        if (dist(mt) < n)
            pkt.flip(i);
    }

    return pkt;
}
