#include "tx.hpp"

#include <cassert>
#include <cstdio>
#include "util.hpp"

Transmitter::Transmitter(const std::filesystem::path& file_path)
    : m_file(fopen(file_path.c_str(), "r")) {
    assert(m_file != NULL);
}

Transmitter::~Transmitter() {
    fclose(m_file);
}

void Transmitter::read_packet() noexcept {
    if (m_current_packet + 1 < m_cache_in_use) {
        m_current_packet++;
        return;
    }

    size_t i = 0;
    for (; i < CACHE_SIZE; i++) {
        if (int c; (c = getc(m_file)) != EOF) {
            m_packet_cache[i] = c;
            m_packet_cache[i][CHECKSUM_BIT] = checksum(c);
        } else {
            fseek(m_file, 0, SEEK_SET);
            m_packet_cache[i] = Packet(0);
            ++i; // inc so cache_in_use is correct
            break;
        }
    }

    m_cache_in_use = i;
    m_current_packet = 0;
}
