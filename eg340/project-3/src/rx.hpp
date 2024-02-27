#pragma once

#include <atomic>
#include <fstream>
#include "util.hpp"

class Receiver {
public:
    Receiver();

    void receive(Packet& packet) noexcept;

private:
    std::ofstream m_file;
    static std::atomic_uint s_file_iteration;
    bool m_file_just_created = true;
};
