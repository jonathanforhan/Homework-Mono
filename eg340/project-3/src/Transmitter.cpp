#include "Transmitter.hpp"

Packet Transmitter::transmit() {
    _file.get(_current_char);
    return retransmit();
}

Packet Transmitter::retransmit() {
    Packet pkt(_current_char);

    if (_file.eof() || _file.fail()) {
        _eot = true;
        return Packet(0U);
    }

    pkt.set_chkbit(Packet::checksum(pkt));

    return pkt;
}
