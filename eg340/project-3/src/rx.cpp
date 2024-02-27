#include "rx.hpp"
#include "util.hpp"

#include <string>

std::atomic_uint Receiver::s_file_iteration = 0;

Receiver::Receiver() {
    m_file.open((std::string("bible_") + std::to_string(s_file_iteration++) + ".txt"));
}

void Receiver::receive(Packet& packet) noexcept {
    if (packet == Packet(0)) {
        if (!m_file_just_created) {
            m_file_just_created = true;
            m_file = std::ofstream();
            m_file.open((std::string("bible_") + std::to_string(s_file_iteration++) + ".txt"));
            packet[ACK_BIT] = 1;
        }
        return;
    }
    m_file_just_created = false;

    auto sum = checksum(static_cast<char>(packet.to_ulong()));
    packet[ACK_BIT] = sum == packet[CHECKSUM_BIT];

    if (packet[ACK_BIT]) {
        m_file << static_cast<char>(packet.to_ulong());
    }
}
