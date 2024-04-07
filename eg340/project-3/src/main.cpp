/// @file main.cpp
/// @brief run the channel tests, meant to be piped from stdout to csv file

#include <iostream>
#include <string>
#include "Channel.hpp"

#define SCHEMA                                                                                                      \
    "noise", "transmissions", "retransmissions", "good-true", "good-false", "bad-true", "bad-false", "first-tries", \
        "catches", "tx-errors", "rx-errors"

/// @brief QOL csv to stdout
/// @param e element
void csv(auto e) {
    std::cout << e << '\n';
}

/// @brief QOL csv to stdout (recursive)
/// @param e element
/// @param rest rest of args, for packing
void csv(auto e, auto... rest) {
    std::cout << e << ',';
    csv(rest...);
}

/// @brief run channel transmission and run export tracker
/// @param i iteration
/// @param export_fn export function, takes tracker as param
void broadcast(int i, auto export_fn) {
    Transmitter tx("res/test.txt");
    Receiver rx(std::string("output_") + std::to_string(i) + ".txt", true);
    Channel ch(i);

    ch.communicate(tx, rx);
    export_fn(ch.tracker);
}

int main() {
    csv(SCHEMA);

    for (int i = 0; i <= 50; i += 1) {
        broadcast(i, [](const PerfLog& tracker) {
            csv(tracker.noise,
                tracker.transmissions,
                tracker.retransmissions,
                tracker.good_true,
                tracker.good_false,
                tracker.bad_true,
                tracker.bad_false,
                tracker.first_tries,
                tracker.catches,
                tracker.tx_errors,
                tracker.rx_errors);
        });
    }

    return 0;
}
