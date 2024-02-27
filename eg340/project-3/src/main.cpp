#include <atomic>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>
#include "rx.hpp"
#include "tx.hpp"
#include "util.hpp"

#define ITERATIONS_PER_THREAD 4'451'368 /* Size of one bible */
#define THREAD_COUNT 64

std::atomic_uint transmissions = 0;
std::atomic_uint errors = 0;
std::atomic_uint catches = 0;

void broadcast(uint32_t prob_error) {
    auto tx = Transmitter("res/bible.txt");
    auto rx = Receiver();

    uint32_t local_transmissions = 0;
    uint32_t local_errors = 0;
    uint32_t local_catches = 0;

    std::vector<uint32_t> hash_cache;
    hash_cache.reserve(CACHE_SIZE);

    for (int i = 0; i < ITERATIONS_PER_THREAD; i++) {
        tx.read_packet();

        const Packet& ideal_packet = tx.transmit();
        bool error_in_scope = false;
        Packet packet;

        do {
            local_transmissions++;
            bool error_this_iter = false;

            if (hash_cache.size() < 2) {
                for (int i = 0; i < CACHE_SIZE; i++)
                    hash_cache.emplace_back(pcg_hash() % 100);
            }

            packet = tx.transmit();
            scramble(packet, prob_error, hash_cache.back());
            hash_cache.pop_back();

            if (packet != ideal_packet) {
                error_in_scope = true;
                error_this_iter = true;
            }

            rx.receive(packet);
            scramble(packet, prob_error, hash_cache.back());
            hash_cache.pop_back();

            if (static_cast<char>(packet.to_ulong()) != static_cast<char>(ideal_packet.to_ulong())) {
                error_in_scope = true;
                error_this_iter = true;
            }

            tx.receive(packet);

            local_errors += static_cast<uint32_t>(error_this_iter);
        } while (tx.transmitting());

        if (error_in_scope && static_cast<char>(packet.to_ulong()) == static_cast<char>(ideal_packet.to_ulong())) {
            local_catches++;
        }
    }

    transmissions += local_transmissions;
    errors += local_errors;
    catches += local_catches;
}

int main() {
    for (int err = 0; err < 101; err += 10) {
        std::cout << "Error: " << err << "%\n";
        std::vector<std::thread> threads;

        for (int i = 0; i < THREAD_COUNT; i++)
            threads.emplace_back(broadcast, err);

        for (auto& thread : threads)
            thread.join();

        std::cout << "Total Bytes Transmitted: " << THREAD_COUNT * ITERATIONS_PER_THREAD << '\n';
        std::cout << "Total Transmissions: " << transmissions << '\n';
        std::cout << "Total Retransmissions: " << (transmissions - ITERATIONS_PER_THREAD * THREAD_COUNT) << '\n';
        std::cout << "Total Incorrect Transmissions: " << errors << '\n';
        std::cout << "Total Corrected Transmissions: " << catches << '\n';
        std::cout << '\n';
    }
}
