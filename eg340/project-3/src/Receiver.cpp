#include "Receiver.hpp"

std::optional<Packet> Receiver::receive(Packet pkt) {
    if (pkt[Packet::CHKBIT] != Packet::checksum(pkt)) {
        pkt.set_ackbit();
        return pkt;
    }
    if (_write)
        _fout << pkt.to_char();
    return std::nullopt;
}
