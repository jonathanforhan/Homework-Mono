/// @file main.cpp
/// @brief run the channel tests, meant to be piped from stdout to csv file

#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
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
void broadcast(int i, std::promise<PerfLog>&& promise) {
    Transmitter tx("res/bible.txt");
    Receiver rx(std::string("output_") + std::to_string(i) + ".txt");
    Channel ch(i);

    ch.communicate(tx, rx);
    promise.set_value(ch.tracker);
}

int main() {
    std::vector<std::future<PerfLog>> futures;
    std::vector<std::thread> threads;

    csv(SCHEMA);

    for (int i = 0; i <= 50; i += 1) {
        std::promise<PerfLog> promise;
        futures.emplace_back(promise.get_future());
        threads.emplace_back(std::thread(broadcast, i, std::move(promise)));
    }

    for (auto& future : futures) {
        auto tracker = future.get();
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
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
